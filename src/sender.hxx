// sender.hxx

#ifndef _sender_hxx
#define _sender_hxx

#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <boost/uuid/uuid.hpp>

class sender
{
public:
  sender(
    boost::asio::io_service& io_service,
    const boost::asio::ip::address& multicast_address,
    boost::uuids::uuid uuid
  );

  void send_message(std::string msg);
  void send_discover();

  void handle_send_to(const boost::system::error_code& error);
  void handle_timeout(const boost::system::error_code& error);

private:
  boost::asio::ip::udp::endpoint endpoint_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::deadline_timer timer_;
  int message_count_;
  std::string message_;
  boost::uuids::uuid uuid_;
};

#endif
