// client.hxx

#ifndef _client_hxx
#define _client_hxx

#include <boost/asio.hpp>
#include <memory>

#include "sender.hxx"
#include "receiver.hxx"

class client
{
public:
  client(boost::asio::io_service& io_service, bool sending);
  void start();

private:
  std::unique_ptr<sender> psender_;
  std::unique_ptr<receiver> preceiver_;
};

#endif
