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
    typedef std::function<void()> on_discover_t;
    typedef std::function<void()> on_message_t;

    sender(
        boost::asio::io_service& io_service,
        const boost::asio::ip::address& multicast_address,
        boost::uuids::uuid uuid
    );

    ~sender();

    void send_message(std::string msg);
    void send_discover(on_discover_t on_discover);
    void send_message_to_peers(std::string message, on_message_t on_message);

    inline std::vector<std::string>* get_discovered() { return &discovered_; }
    inline std::vector<std::string>* get_responses() { return &responses_; }
    inline std::vector<std::string>* get_responding_peers() { return &responding_peers_; }

    void handle_send_to(const boost::system::error_code& error);

    void handle_discover_timeout(const boost::system::error_code& error);
    void handle_response_timeout(const boost::system::error_code& error);

private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;

    boost::asio::deadline_timer discover_timer_;
    boost::asio::deadline_timer response_timer_;

    int message_count_;
    std::string message_;
    boost::uuids::uuid uuid_;

    std::vector<std::string> discovered_;

    std::vector<std::string> responses_;
    std::vector<std::string> responding_peers_;

    on_discover_t on_discover_;
    on_message_t on_message_;
};

#endif
