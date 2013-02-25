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
      timer_(io_service),
      message_count_(0),
      uuid_(uuid)

  {
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
void sender::send_discover()
{
    string msg("discover " + boost::uuids::to_string(uuid_));
    send_message(msg);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void sender::handle_send_to(const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << "message sent successfully" << std::endl;
    }

#if 0
    if (!error && message_count_ < max_message_count)
    {
      timer_.expires_from_now(boost::posix_time::seconds(1));
      timer_.async_wait(
          boost::bind(&sender::handle_timeout, this,
            boost::asio::placeholders::error));
    }
#endif
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sender::handle_timeout(const boost::system::error_code& error)
  {
    if (!error)
    {
      std::ostringstream os;
      os << "Message " << message_count_++;
      message_ = os.str();

      socket_.async_send_to(
          boost::asio::buffer(message_), endpoint_,
          boost::bind(&sender::handle_send_to, this,
            boost::asio::placeholders::error));
    }
  }
