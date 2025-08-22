#include "connection.hpp"
#include "output.hpp"

connection::connection() : endpoint(serverIp, serverPort){};
    
std::string connection::send(std::string data){

    tcp::socket socket(io);
    socket.connect(endpoint, error);

    if(error || !socket.is_open()){
        outputLink->notificationHide();
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

int8_t connection::sendMessage(std::string message){
         
    json jsonMessage;

    jsonMessage["code"] = 1001;
    jsonMessage["message"] = message;

    message = jsonMessage.dump(-1);

    outputLink->notification("Подключение...", "", 0);

    try{
        std::string result = send(message);

        outputLink->notificationHide();

        json reply = json::parse(result);

        if (reply["code"] == 200){
            outputLink->notification("OK", "code: 200", 1);
        } else {
            int16_t code = reply["code"];
            std::string strCode = "code: " + std::to_string(code);
            outputLink->error("ERROR", strCode);
        }

    } catch (boost::system::system_error& e){
        outputLink->error("Ошибка подключения", error.message());
        return 1;
    } catch(std::exception &e){
        outputLink->error("Ошибка", e.what());
        return 1;
    } catch(...){
        outputLink->error("Произошла неизвестная ошибка", "");
        return 1;
    }

    return 0;
}

json connection::get(){

    std::string message; 
    json jsonMessage;

    jsonMessage["code"] = 1002;
    message = jsonMessage.dump(-1);

    outputLink->notification("Подключение...", "", 0);

    try{
        std::string result = send(message);

        outputLink->notificationHide();

        json reply;
        reply["data"] = json::parse(result);
        reply["success"] = 0;

        return reply;

    } catch (boost::system::system_error& e){
        outputLink->error("Ошибка подключения", error.message());
        return {{"success", 1}};
    } catch(std::exception &e){
        outputLink->error("Ошибка", e.what());
        return {{"success", 1}};
    } catch(...){
        outputLink->error("Произошла неизвестная ошибка", "");
        return {{"success", 1}};
    }
}

int8_t connection::addOutput(output* outputI){
    outputLink = outputI;
    return 0;
}