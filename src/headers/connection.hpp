#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using boost::asio::ip::tcp;
using json = nlohmann::json;
namespace asio = boost::asio;

class connection{
private:
    boost::system::error_code error;
    asio::io_context io;
    const asio::ip::address serverIp = asio::ip::make_address("127.0.0.1");
    const int serverPort = 12345;
    tcp::endpoint endpoint;

public:
    connection();
    std::string send(std::string data);
    int sendMessage(std::string message);
    json get();
};

#endif