// File: receiver.hxx

// Adapted from here: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/multicast/receiver.cpp

#ifndef _receiver_hxx
#define _receiver_hxx

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

class receiver
{
public:
  receiver(boost::asio::io_service& io_service,
      const boost::asio::ip::address& listen_address,
      const boost::asio::ip::address& multicast_address);

  void handle_receive_from(const boost::system::error_code& error,
      size_t bytes_recvd);

private:
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
};

#endif