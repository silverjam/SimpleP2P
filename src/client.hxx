// client.hxx

#ifndef _client_hxx
#define _client_hxx

#include <memory>

#include <boost/asio.hpp>

#include "sender.hxx"
#include "receiver.hxx"

class client
{
public:
  client(boost::asio::io_service& io_service);
  inline std::shared_ptr<sender> get_sender() { return psender_; }

private:
  std::shared_ptr<sender> psender_;
  std::unique_ptr<receiver> preceiver_;
};

#endif
