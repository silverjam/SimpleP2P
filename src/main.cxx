// main.cxx

#include "client.hxx"
#include "simplep2p.hxx"

int main(int argc, char* argv[])
{
    using namespace std;

#ifdef NDEBUG
    try
#endif
    {

        if ( argv[1] == string("send") )
        {
            simplep2p simp;
            simp.discover_peers();

            simp.send_message_to_peers("hello");

            if ( simp.response_count() != simp.get_peer_count() )
            {
                std::cout << "FAIL" << std::endl;
            }
            else
            {
                std::cout << "PASS" << std::endl;
            }
        }
        else
        {
            boost::asio::io_service io_service;

            unique_ptr<client> the_client;
            the_client = unique_ptr<client>(new client(io_service));

            io_service.run();
        }
    }
#ifdef NDEBUG
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
#endif

    return 0;
}
