#include "headers.hpp"

size_t headers::sizeUTF8(const std::string string) {
    size_t size = 0;
    int16_t sort = 0;
    while (sort < string.size()) {
        if ((string[sort] & 0x80) == 0x00){
            sort = sort + 1;
        } else if ((string[sort] & 0xE0) == 0xC0){
            sort = sort + 2;
        } else if ((string[sort] & 0xF0) == 0xE0){
            sort = sort + 3;
        } else if ((string[sort] & 0xF8) == 0xF0){
            sort = sort + 4;
        } 
        size++;
    }
    return size;
}