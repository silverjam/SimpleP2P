// File: receiver.hxx

// Adapted from here: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/multicast/receiver.cpp

#ifndef _receiver_hxx
#define _receiver_hxx

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>

#include "sender.hxx"

class receiver
{
public:
  receiver(
    boost::asio::io_service& io_service,
    const boost::asio::ip::address& listen_address,
    const boost::asio::ip::address& multicast_address,
    boost::uuids::uuid uuid,
    std::shared_ptr<sender> psender
    );

  ~receiver();

  void handle_receive_from(const boost::system::error_code& error,
      size_t bytes_recvd);

private:
  typedef boost::shared_ptr<boost::asio::ip::udp::socket> socket_ptr;

  socket_ptr socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;

  enum { max_length = 1024 };
  char data_[max_length];

  const boost::asio::ip::address& multicast_address_;

  boost::uuids::uuid uuid_;
  std::shared_ptr<sender> sender_;
};

#endif
