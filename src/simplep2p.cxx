// simplep2p.cxx

#include "client.hxx"
#include "sender.hxx"
#include "receiver.hxx"

#include "simplep2p.hxx"

using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class simplep2p_private
{
public:
    simplep2p_private()
    {
    }

    void reset()
    {
        if ( client_.get() != 0 )
        {
            client_.reset();
            io_service_.reset();
        }

        io_service_ = unique_ptr<boost::asio::io_service>(new boost::asio::io_service);
        client_ = unique_ptr<client>(new client(*io_service_.get()));
    }

    void send_message_to_peers(std::string message)
    {
        reset();

        responding_peers_.clear();
        responses_.clear();

        sender::on_message_t on_message = [&] ()
        {
            vector<string>& responses = *(client_->get_sender()->get_responses());
            vector<string>& peers = *(client_->get_sender()->get_responding_peers());

            for (int x = 0; x < responses.size(); x++)
            {
                responding_peers_.push_back(peers[x]);
                responses_.push_back(responses[x]);
            }

            io_service_->stop();
        };

        client_->get_sender()->send_message_to_peers(message, on_message);
        io_service_->run();
    }

    void discover_peers()
    {
        reset();
        peerlist_.clear();

        sender::on_discover_t on_disco = [&] ()
        {
            vector<string>& discovered = *(client_->get_sender()->get_discovered());

            for (int x = 0; x < discovered.size(); x++)
                peerlist_.push_back(discovered[x]);

            io_service_->stop();
        };

        client_->get_sender()->send_discover(on_disco);
        io_service_->run();
    }

    unique_ptr<boost::asio::io_service> io_service_;
    unique_ptr<client> client_;

    vector<string> peerlist_;
    vector<string> responding_peers_;
    vector<string> responses_;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
simplep2p::simplep2p()
    : d(new simplep2p_private)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
simplep2p::~simplep2p()
{
    delete d;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void simplep2p::discover_peers()
{
    d->discover_peers();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int simplep2p::get_peer_count()
{
    return d->peerlist_.size();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* simplep2p::get_peer(int index)
{
    return d->peerlist_[index].c_str();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void simplep2p::send_message_to_peers(const char* cmessage)
{
    std::string message(cmessage);
    d->send_message_to_peers(message);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int simplep2p::response_count()
{
    return d->responses_.size();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* simplep2p::get_peer_of_response(int index)
{
    return d->responding_peers_[index].c_str();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* simplep2p::get_text_of_response(int index)
{
    return d->responses_[index].c_str();
}
