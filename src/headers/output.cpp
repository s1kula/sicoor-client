#include "output.hpp"

output::output(connection* connectionLinkI) : connectionLink(connectionLinkI) {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    initscr();

    getmaxyx(stdscr, row, col);
    menuWin = newwin(row, col, 0, 0);
    sendWin = newwin(row, col, 0, 0);
    getWin = newwin(row, col, 0, 0);

    menuPanel = new_panel(menuWin);
    sendPanel = new_panel(sendWin);
    getPanel = new_panel(getWin);

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

    getmaxyx(stdscr, row, col);
    row--;

    json data = connectionLink->get();
    for(auto dataIterator = data.rbegin(); dataIterator != data.rend(); ++dataIterator, --row){
        std::string message = (*dataIterator)["message"]; 
        message = message.substr(0, col);
        const char* charMessage = message.c_str();

        mvwprintw(getWin, row, 0, "%s\n", charMessage);
    }

    update_panels();
    doupdate();

    getch();

    return 0;
}