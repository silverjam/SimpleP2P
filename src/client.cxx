// client.cxx

#include <string>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "client.hxx"

const std::string LISTEN_ADDRESS = "0.0.0.0";
const std::string MULTICAST_ADDR = "239.255.0.1";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

client::client(boost::asio::io_service& io_service)
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    {
        auto address = boost::asio::ip::address::from_string(MULTICAST_ADDR);

        psender_ =
            std::shared_ptr<sender>(
                new sender(io_service, address, uuid)
            );
    }

    {
        auto listen_addr = boost::asio::ip::address::from_string(LISTEN_ADDRESS);
        auto multicast_addr = boost::asio::ip::address::from_string(MULTICAST_ADDR);

        preceiver_ =
            std::unique_ptr<receiver>(
                new receiver(io_service, listen_addr, multicast_addr, uuid, psender_)
            );
    }
}
