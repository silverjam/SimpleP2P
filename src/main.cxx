// main.cxx

#include "client.hxx"
#include "simplep2p.hxx"

void print_usage(char* argv[])
{
    std::cerr << "Usage: " << argv[0] << " <\"send\" | \"recv\">" << std::endl;
}

int main(int argc, char* argv[])
{
    using namespace std;

#ifdef NDEBUG
    try
#endif
    {
        if ( argc < 2 )
        {
            print_usage(argv);
            return 1;
        }
        
        if ( argv[1] == string("send") )
        {
            std::cout << "Sending..." << std::endl;

            simplep2p simp;
            simp.discover_peers();

            if ( simp.get_peer_count() == 0 )
            {
                std::cout << "FAIL: NO PEERS!" << std::endl;
                return 0;
            }

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
        else if ( argv[1] == string("recv") )
        {
            boost::asio::io_service io_service;

            unique_ptr<client> the_client;
            the_client = unique_ptr<client>(new client(io_service));

            io_service.run();
        }
        else
        {
            print_usage(argv);
            return 1;
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
