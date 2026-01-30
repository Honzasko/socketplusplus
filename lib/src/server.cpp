#include "../include/server.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "../include/RequestHandler.hpp"
#include <functional>
#include "../include/scheduler.hpp"

Socket::Server::Server(int port, char type,char IP){
    ipv6 = false;
    this->socket_fd = socket((IP == IPV4) ? AF_INET : AF_INET6, (type == TYPE_TCP) ? SOCK_STREAM : SOCK_DGRAM, 0);
    if(socket_fd < 0) {
        std::cout << "Socket creation failed" << std::endl;
        return;
    }
    if(IP == IPV6) ipv6 = true;
    opt = 1;

    if(setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(opt))) {
        std::cout << "Failed to set socket opt" << std::endl;
        return;
    }

    if(ipv6){
        serv_addr6.sin6_family = AF_INET6;
        serv_addr6.sin6_addr = in6addr_any;
        serv_addr6.sin6_port = htons(port);
    }
    else {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

    }

    

}

void Socket::Server::Listen(std::function<RequestHandler*()> handlerFactory){
    int stat = 0;
    struct sockaddr* serv;
    int  size;
    if(ipv6){
        stat = bind(this->socket_fd, (struct sockaddr*)&serv_addr6, sizeof(sockaddr_in6));

    }
    else {
        stat = bind(this->socket_fd, (struct sockaddr*)&serv_addr, sizeof(sockaddr_in));
    }

    if(stat < 0) {
        perror("Bind failed");
        return;
    }

    if(listen(this->socket_fd, 10) < 0){
        std::cout << "Listening failed" << std::endl;
        return;
    }

    std::cout << "Listening"  << std::endl; 
    struct sockaddr_storage storage;
    socklen_t addrlen = sizeof(storage);

    while (true) {
        int accept_fd;
        accept_fd = accept(this->socket_fd, (struct sockaddr*)&storage, &addrlen);
        if(accept_fd < 0) continue;

        RequestHandler* client_obj = handlerFactory();
        SchedulerClient client;
        client.handler = client_obj;
        client.fd = accept_fd;
        scheduler.addClient(client);
      
    }
}
