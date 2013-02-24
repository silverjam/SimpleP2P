// client.hxx

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class client_private;

class client
{
public:
  client(boost::asio::io_service& io_service);
  tcp::socket& socket();

  void start();

private:
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);

  client_private* d;
};
