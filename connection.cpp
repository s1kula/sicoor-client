#include "connection.hpp"

connection::connection() : endpoint(serverIp, serverPort){};
    
std::string connection::send(std::string data){

    tcp::socket socket(io);
    socket.connect(endpoint, error);

    if(error || !socket.is_open()){
        throw error;
    }

    uint32_t lenWriteBuffer = data.size();

    asio::write(socket, asio::buffer(&lenWriteBuffer, 4));
    asio::write(socket, asio::buffer(data));

    uint32_t lenReadBuffer;
    asio::read(socket, asio::buffer(&lenReadBuffer, 4));

    std::string readBuffer; 
    readBuffer.resize(lenReadBuffer);
    asio::read(socket, asio::buffer(readBuffer));

    socket.close();

    return readBuffer;
} 

int connection::sendMessage(){
        
    std::string writeBuffer, message; 
    json jsonMessage;

    std::cout << "напишите сообщение:" << std::endl;
    std::cin.ignore();
    std::getline(std::cin, writeBuffer);

    jsonMessage["code"] = 1001;
    jsonMessage["message"] = writeBuffer;

    message = jsonMessage.dump(-1);

    std::cout << "подключение..." << std::endl;

    try{
        std::string result = send(message);

        json reply = json::parse(result);

        std::cout << reply["code"] << std::endl;

    } catch (boost::system::system_error& e){
        std::cout << "ошибка подключения " << error.message() << std::endl;
        return 1;
    } catch(std::exception &e){
        std::cout << "ошибка " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int connection::get(){

    std::string message; 
    json jsonMessage;

    jsonMessage["code"] = 1002;
    message = jsonMessage.dump(-1);

    std::cout << "подключение..." << std::endl;

    try{
        std::string result = send(message);

        json reply = json::parse(result);

        for(const json data : reply){
            std::string message = data["message"]; 
            std::cout << message << std::endl;
        }

    } catch (boost::system::system_error& e){
        std::cout << "ошибка подключения " << error.message() << std::endl;
        return 1;
    } catch(std::exception &e){
        std::cout << "ошибка " << e.what() << std::endl;
        return 1;
    }

    return 0;
}