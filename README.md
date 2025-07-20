для работы клиента измените ip и port на строке 91-92 на данные вашего сервера. по умолчанию установлен ip localhost

To make the client work, change the IP and port in lines 91-92 to your server data. By default, ip localhost is set


сборка/build

g++ main.cpp -o main -lboost_system


зависимости/dependencies

boost/asio              (boost.org)
nlohmann-json           (github.com/nlohmann/json)