#include <iostream>
#include "headers/connection.hpp"
#include "headers/output.hpp"

int main(){

    connection connection;
    output output(&connection);
    
    connection.addOutput(&output);

    while (1){
        try{
            output.menu();
        }catch(...){
            output.error("Произошла неизвестная ошибка","");
        }
    }
    
    endwin();

    return 0;
}