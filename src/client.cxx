// client.cxx

#include <string>

#include "client.hxx"

const std::string LISTEN_ADDRESS = "0.0.0.0";
const std::string MULTICAST_ADDR = "239.255.0.1";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

client::client(boost::asio::io_service& io_service, bool sending)
{
    if ( sending )
    {
        psender_ = std::unique_ptr<sender>(new sender(io_service,
              boost::asio::ip::address::from_string(MULTICAST_ADDR)));
    }
    else
    {
        preceiver_ = std::unique_ptr<receiver>(new receiver(io_service,
                boost::asio::ip::address::from_string(LISTEN_ADDRESS),
                boost::asio::ip::address::from_string(MULTICAST_ADDR)));
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void client::start()
{
}
