#include <iostream>
#include "connection.hpp"

int main(){

    connection connection;

    while (1){
        try{
            std::cout << "q - написать сообщение\t\tw - получить сообщения" << std::endl;
            char buf;
            std::cin >> buf;
            buf = tolower(buf);

            if(buf == 'q'){
                connection.sendMessage();
            } else if(buf == 'w'){
                connection.get();
            }
        }catch(...){
            std::cout << "произошла неизвестная ошибка" << std::endl;
        }
    }
    
    return 0;
}