// simplep2p.cxx

#include "client.hxx"

int main(int argc, char* argv[])
{
    using namespace std;

#ifdef NDEBUG
    try
#endif
    {
        boost::asio::io_service io_service;
        unique_ptr<client> the_client;

        if ( argv[1] == string("send") )
        {
            the_client = unique_ptr<client>(new client(io_service));
            the_client->get_sender()->send_discover();
        }
        else
        {
            the_client = unique_ptr<client>(new client(io_service));
        }

        io_service.run();
    }
#ifdef NDEBUG
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
#endif

    return 0;
}
