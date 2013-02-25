// File: receiver.cxx

#include "receiver.hxx"
#include "sender.hxx"

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <boost/uuid/uuid_io.hpp>

#include "sender.hxx"

using namespace std;
using namespace boost;

const short multicast_port = 30001;

receiver::receiver(boost::asio::io_service& io_service,
                   const boost::asio::ip::address& listen_address,
                   const boost::asio::ip::address& multicast_address,
                   boost::uuids::uuid uuid,
                   std::shared_ptr<sender> psender)

    : socket_(io_service)
    , multicast_address_(multicast_address)
    , uuid_(uuid)
    , sender_(psender)

  {
    // Create the socket so that multiple may be bound to the same address.
    boost::asio::ip::udp::endpoint listen_endpoint(
        listen_address, multicast_port);
    socket_.open(listen_endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(listen_endpoint);

    // Join the multicast group.
    socket_.set_option(
        boost::asio::ip::multicast::join_group(multicast_address));

    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&receiver::handle_receive_from, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

void string_split(string input, vector<string>& output)
{
    istringstream iss(input);

    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(output));
}

void receiver::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
  {
    if (!error)
    {
        std::ostringstream os;
        os.write(data_, bytes_recvd);

        vector<string> message;
        string_split(os.str(), message);

        if ( message[0] == "discover" )
        {
            sender_->send_message("discovered " + message[1] + " " + uuids::to_string(uuid_));
        }
        else if ( message[0] == "discovered" )
        {
            std::cout << os.str() << std::endl;
            std::string uuid_str = uuids::to_string(uuid_);

            // check if message is to us
            if ( message[1] == uuid_str )
            {
                // don't add if we sent this
                if ( message[2] != uuid_str )
                    sender_->get_discovered()->push_back(message[2]);
            }
        }

        // re-issue
        socket_.async_receive_from(
            boost::asio::buffer(data_, max_length), sender_endpoint_,
            boost::bind(&receiver::handle_receive_from, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
  }
