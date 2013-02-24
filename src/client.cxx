// client.cxx

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "client.hxx"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class client_private
{
public:
  client_private(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

client::client(boost::asio::io_service& io_service)
    : d(new client_private(io_service))
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tcp::socket& client::socket()
{
    return d->socket_;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void client::start()
{
    d->socket_.async_read_some(boost::asio::buffer(d->data_, d->max_length),
	boost::bind(&client::handle_read, this,
	  boost::asio::placeholders::error,
	  boost::asio::placeholders::bytes_transferred));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void client::handle_read(const boost::system::error_code& error,
  size_t bytes_transferred)
{
    if (!error)
    {
      boost::asio::async_write(d->socket_,
	  boost::asio::buffer(d->data_, bytes_transferred),
	  boost::bind(&client::handle_write, this,
	    boost::asio::placeholders::error));
    }
    else
    {
      delete this;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void client::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
      d->socket_.async_read_some(boost::asio::buffer(d->data_, d->max_length),
	  boost::bind(&client::handle_read, this,
	    boost::asio::placeholders::error,
	    boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
}
