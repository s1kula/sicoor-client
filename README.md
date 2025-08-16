для работы клиента измените ip и port на строке 14-15 (connection.hpp) на данные вашего сервера. по умолчанию установлен ip localhost

To make the client work, change the IP and port in lines 14-15 (connection.hpp) to your server data. By default, ip localhost is set


сборка/build

g++ main.cpp headers/connection.cpp headers/output.cpp -o main -lboost_system -lpanel -lncurses


зависимости/dependencies

boost/asio              (boost.org)
nlohmann-json           (github.com/nlohmann/json)
ncurses                 (invisible-island.net/ncurses)