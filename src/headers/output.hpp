#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <clocale>
#include "connection.hpp"
#include <ncurses.h>
#include <panel.h>
#include <string>
#include <nlohmann/json.hpp>
#include "headers.hpp"

#define MENU_NUM 1
#define SEND_NUM 2
#define GET_NUM 3
#define NOTIFICATION_NUM 4

#define NOTIFICATION_ROW 10
#define NOTIFICATION_COL 60

class output{
private:
    connection* connectionLink; 
    WINDOW *menuWin, *sendWin, *getWin, *notificationWin;
    PANEL *menuPanel, *sendPanel, *getPanel, *notificationPanel;
    int16_t row, col;
    int8_t page;
public:
    output(connection* connectionLinkI);
    int8_t menu();
    int8_t send();
    int8_t get();
    int8_t notification(std::string title, std::string description, bool waitChar);
    int8_t notificationHide();
    int8_t error(std::string title, std::string description);
};

#endif