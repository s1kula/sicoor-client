#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace asio = boost::asio;

int send(asio::io_context* io, tcp::endpoint* endpoint, boost::system::error_code* error){

    std::cout << "подключение..." << std::endl;

    tcp::socket socket(*io);
    socket.connect(*endpoint, *error);

    if(*error || !socket.is_open()){
        std::cout << "ошибка подключения " << error->message() << std::endl;
        return 1;
    }

    std::string writeBuffer, message;
    std::cout << "напишите сообщение:" << std::endl;
    std::cin.ignore();
    std::getline(std::cin, writeBuffer);
    message = "<SEND>" + writeBuffer;

    uint32_t lenWriteBuffer = message.size();

    asio::write(socket, asio::buffer(&lenWriteBuffer, 4));
    asio::write(socket, asio::buffer(message));

    uint32_t lenReadBuffer;
    asio::read(socket, asio::buffer(&lenReadBuffer, 4));

    std::string readBuffer; 
    readBuffer.resize(lenReadBuffer);
    asio::read(socket, asio::buffer(readBuffer));

    std::cout << readBuffer << std::endl;

    socket.close();

    return 0;
}

int get(asio::io_context* io, tcp::endpoint* endpoint, boost::system::error_code* error){

    std::cout << "подключение..." << std::endl;

    tcp::socket socket(*io);
    socket.connect(*endpoint, *error);

    if(*error || !socket.is_open()){
        std::cout << "ошибка подключения " << error->message() << std::endl;
        return 1;
    }

    std::string query = "<GET>";

    uint32_t lenQuery= query.size();

    asio::write(socket, asio::buffer(&lenQuery, 4));
    asio::write(socket, asio::buffer(query));

    std::cout << "запрос сообщений с сервера..." << std::endl;

    uint32_t lenReadBuffer;
    asio::read(socket, asio::buffer(&lenReadBuffer, 4));

    std::string readBuffer; 
    readBuffer.resize(lenReadBuffer);
    asio::read(socket, asio::buffer(readBuffer));

    json allData = json::parse(readBuffer);

    for(const json data : allData){
        std::string message = data["message"]; 
        std::cout << message << std::endl;
    }

    socket.close();

    return 0;
}

int main(){
    boost::system::error_code error;
    asio::io_context io;
    asio::ip::address serverIp = asio::ip::make_address("127.0.0.1");
    tcp::endpoint endpoint(serverIp , 12345);

    while (1){
        try{
            std::cout << "q - написать сообщение\t\tw - получить сообщения" << std::endl;
            char buf;
            std::cin >> buf;
            buf = tolower(buf);

            if(buf == 'q'){
                send(&io, &endpoint, &error);
            } else if(buf == 'w'){
                get(&io, &endpoint, &error);
            }
        }catch(...){
            std::cout << "произошла неизвестная ошибка" << std::endl;
        }
    }
    
    return 0;
}