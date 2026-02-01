#include <server/server.hpp>
#include "http.hpp"

int main (){
    Socket::Server srv = Socket::Server(5000, TYPE_TCP, IPV4);

    srv.Listen([]() {
        return new Http(); 
    });

    return 0;
}