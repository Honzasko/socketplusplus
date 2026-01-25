#include "include/server.hpp" 
#include "chat.hpp"

int main (){
    Socket::Server srv = Socket::Server(5000, TYPE_TCP, IPV4);

    srv.Listen([]() {
        return new Chat(); 
    });

    return 0;
}