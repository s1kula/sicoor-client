#include "output.hpp"

output::output(connection* connectionLinkI) : connectionLink(connectionLinkI) {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    initscr();

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);

    getmaxyx(stdscr, row, col);
    menuWin = newwin(row, col, 0, 0);
    sendWin = newwin(row, col, 0, 0);
    getWin = newwin(row, col, 0, 0);
    notificationWin = newwin(row, col, 0, 0);

    menuPanel = new_panel(menuWin);
    sendPanel = new_panel(sendWin);
    getPanel = new_panel(getWin);
    notificationPanel = new_panel(notificationWin);

    wattron(menuWin, A_STANDOUT | A_BOLD);
};

int8_t output::menu(){

    page = MENU_NUM;

    top_panel(menuPanel);

    curs_set(0);
    noecho();

    werase(menuWin);
        
    getmaxyx(stdscr, row, col);
        
    mvwprintw(menuWin, row/2-1, (col-48)/2, "                                                ");
    mvwprintw(menuWin, row/2-1, (col-6)/2, "SICOOR");
    mvwprintw(menuWin, row/2, (col-48)/2, "q - написать сообщение    w - получить сообщения");
        
    update_panels();
    doupdate();

    char input = tolower(getch());

    if(input == 'q'){
        send();
    } else if(input == 'w'){
        get();
    }

    return 0;

}

int8_t output::send(){
    
    page = SEND_NUM;

    top_panel(sendPanel);

    curs_set(1);
    echo();

    werase(sendWin);
        
    wprintw(sendWin, "напишите сообщение: ");

    char buffer[2048];

    wgetnstr(sendWin, buffer, 2047);

    std::string message = buffer;

    connectionLink->sendMessage(buffer);

    return 0;
}

int8_t output::get(){
    
    page = GET_NUM;

    top_panel(getPanel);

    curs_set(0);
    noecho();

    werase(getWin);

    json reply = connectionLink->get();

    json data = reply["data"];

    getmaxyx(stdscr, row, col);
    row--;

    if (reply["success"] == 0){
        
        for(auto dataIterator = data.rbegin(); dataIterator != data.rend(); ++dataIterator, --row){
            std::string message = (*dataIterator)["message"]; 
            message = message.substr(0, col);

            mvwprintw(getWin, row, 0, "%s\n", message.c_str());
        }
        
        top_panel(getPanel);

        update_panels();
        doupdate();

        getch();
    }

    return 0;
}

int8_t output::notification(std::string title, std::string description, bool waitChar){    

    int16_t notificationRow, notificationCol;
    int16_t titlePosRow, descriptionPosRow;
    int16_t titlePosCol, descriptionPosCol;

    page = NOTIFICATION_NUM;

    top_panel(notificationPanel);

    curs_set(0);
    noecho();

    werase(notificationWin);

    getmaxyx(stdscr, row, col);

    if(row >= NOTIFICATION_ROW && col >= NOTIFICATION_COL){
        wresize(notificationWin, NOTIFICATION_ROW, NOTIFICATION_COL);
        mvwin(notificationWin, ((row - NOTIFICATION_ROW) / 2), ((col - NOTIFICATION_COL) / 2));
    } else if (row >= NOTIFICATION_ROW){
        wresize(notificationWin, NOTIFICATION_ROW, col);
        mvwin(notificationWin, ((row - NOTIFICATION_ROW) / 2), 0);
    } else if (col >= NOTIFICATION_COL){
        wresize(notificationWin, row, NOTIFICATION_COL);
        mvwin(notificationWin, 0, ((col - NOTIFICATION_COL) / 2));
    } else {
        wresize(notificationWin, row, col);
        mvwin(notificationWin, 0, 0);
    }

    werase(stdscr);
    werase(notificationWin);
    
    box(notificationWin, 0, 0);
    
    getmaxyx(notificationWin, notificationRow, notificationCol);

    titlePosRow = (notificationRow - 1) / 3 ;     //random functions giving the best result
    descriptionPosRow = (notificationRow * 3) / 5 ;

    titlePosCol = (notificationCol - headers::sizeUTF8(title)) / 2 ;
    descriptionPosCol = (notificationCol - headers::sizeUTF8(description)) / 2 ;

    mvwprintw(notificationWin, titlePosRow, titlePosCol, "%s", title.c_str());
    mvwprintw(notificationWin, descriptionPosRow, descriptionPosCol, "%s", description.c_str());
    
    update_panels();
    doupdate();

    if (waitChar){
        getch();
        bottom_panel(notificationPanel);
        update_panels();
        doupdate();
    }

    return 0;
}

int8_t output::notificationHide(){

    bottom_panel(notificationPanel);

    update_panels();
    doupdate();

    return 0;
}

int8_t output::error(std::string title, std::string description){

    wattron(notificationWin, COLOR_PAIR(1) | A_BOLD);
    notification(title, description, 1);
    wattroff(notificationWin, COLOR_PAIR(1) | A_BOLD);

    return 0;
}