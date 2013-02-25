// simplep2p.cxx

#include "client.hxx"

int main(int argc, char* argv[])
{
    using namespace std;

    try
    {
        boost::asio::io_service io_service;
        unique_ptr<client> the_client;

        if ( argv[1] == string("send") )
        {
            the_client = unique_ptr<client>(new client(io_service, true));
        }
        else
        {
            the_client = unique_ptr<client>(new client(io_service, false));
        }

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
