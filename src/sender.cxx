// sender.cpp
//
// adapted from: ttp://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/multicast/sender.cpp

#include <iostream>
#include "boost/uuid/uuid_io.hpp"

#include "sender.hxx"

using namespace std;

const short multicast_port = 30001;
const int max_message_count = 10;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sender::sender(boost::asio::io_service& io_service,
               const boost::asio::ip::address& multicast_address,
               boost::uuids::uuid uuid)

    : endpoint_(multicast_address, multicast_port),
      socket_(io_service, endpoint_.protocol()),
      discover_timer_(io_service),
      response_timer_(io_service),
      message_count_(0),
      uuid_(uuid)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sender::~sender()
{
	socket_.close();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sender::send_message(std::string msg)
{
    std::ostringstream os;
    os << msg;

    message_ = os.str();

    socket_.async_send_to(
        boost::asio::buffer(message_), endpoint_,
        boost::bind(&sender::handle_send_to, this,
          boost::asio::placeholders::error));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sender::send_message_to_peers(std::string message, on_message_t on_message)
{
    on_message_ = on_message; 

    string msg("message " + boost::uuids::to_string(uuid_) + " " + message);

    get_responding_peers()->clear();
    get_responses()->clear();

    send_message(msg);

    response_timer_.expires_from_now(boost::posix_time::seconds(1));
      response_timer_.async_wait(
          boost::bind(&sender::handle_response_timeout, this,
            boost::asio::placeholders::error));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sender::send_discover(on_discover_t on_discover)
{
    on_discover_ = on_discover; 

    string msg("discover " + boost::uuids::to_string(uuid_));

    get_discovered()->clear();
    send_message(msg);

    discover_timer_.expires_from_now(boost::posix_time::seconds(1));
      discover_timer_.async_wait(
          boost::bind(&sender::handle_discover_timeout, this,
            boost::asio::placeholders::error));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sender::handle_send_to(const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << ">>> Message sent successfully" << std::endl;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Handle timeout of the send_discover method, which waits 1 second
//  for peers to respond.
//
void sender::handle_discover_timeout(const boost::system::error_code& error)
{
    if (!error)
    {
#ifdef DEBUG_LOGGING
        for (int x = 0; x < discovered_.size(); x++)
        {
            std::cout << ">>> Discovered peer: " << discovered_[x] << std::endl;
        }
#endif

        on_discover_();
    }
    else
    {
        std::cerr << "!!! " << __FUNCTION__ << ": " << error << std::endl;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Handle timeout of the send_message_to_peer method, which waits
//   1 second for peers to respond.
//
void sender::handle_response_timeout(const boost::system::error_code& error)
{
    if (!error)
    {
#ifdef DEBUG_LOGGING
        for (int x = 0; x < discovered_.size(); x++)
        {
            std::cout << ">>> Message from peer: " << discovered_[x] << std::endl;
        }
#endif

        on_message_();
    }
    else
    {
        std::cerr << "!!! " << __FUNCTION__ << ": " << error << std::endl;
    }

}
