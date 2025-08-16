#include <iostream>
#include "headers/connection.hpp"
#include "headers/output.hpp"

int main(){

    connection connection;
    output output(&connection);

    while (1){
        try{
            output.menu();
        }catch(...){
            //std::cout << "произошла неизвестная ошибка" << std::endl;
        }
    }
    
    endwin();

    return 0;
}