#ifndef OUTPUT_HPP
#define COUTPUT_HPP

#include <clocale>
#include "connection.hpp"
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <nlohmann/json.hpp>

#define MENU_NUM 1
#define SEND_NUM 2
#define GET_NUM 3

class output{
private:
    connection* connectionLink; 
    WINDOW *menuWin, *sendWin, *getWin;
    PANEL *menuPanel, *sendPanel, *getPanel;
    int16_t row, col;
    int8_t page;
public:
    output(connection* connectionLinkI);
    int8_t menu();
    int8_t send();
    int8_t get();
};

#endif