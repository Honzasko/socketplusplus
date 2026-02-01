#include <sys/socket.h>
#define TYPE_TCP 1
#define TYPE_UDP 2
#define IPV4 3
#define IPV6 4

#pragma once
#include <netinet/in.h>
#include "RequestHandler.hpp"
#include <functional>
#include "scheduler.hpp"
#include <server/scheduler/multireactor.hpp>

namespace Socket {
    class Server {
        private:
            int socket_fd;
            bool ipv6;
            bool init = false;
            int opt;
            struct sockaddr_in serv_addr;
            struct sockaddr_in6 serv_addr6;
            void HandleClient(RequestHandler* client_obj, int client,int id);
            Schedulers::Multireactor scheduler;
        public:
            Server(int port, char type,char IP);
            void Listen(std::function<RequestHandler*()> handlerFactory);
    };
}