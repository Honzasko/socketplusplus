#include "../include/scheduler.hpp"
#include "../include/RequestHandler.hpp"
#include <algorithm>
#include <cerrno>
#include <iterator>
#include <sys/types.h>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>

Socket::Scheduler::Scheduler(){
    nproc = std::thread::hardware_concurrency();
    if(nproc < 1) nproc = 1;

    for(int i = 0;i < nproc;i++){
        workers.push_back(epoll_create1(0));
        threads.emplace_back(&Socket::Scheduler::task,this,i);
        workerPool.push_back(0);
    }
}

void Socket::Scheduler::clean(Socket::SchedulerClient* client,int  worker){
    delete client->handler;
    epoll_ctl(workers[worker], EPOLL_CTL_DEL, client->fd, nullptr);
    close(client->fd);
    delete client;
    workerPool[worker]--;
}

void Socket::Scheduler::task(int key){
    while (true) {

        epoll_event events[64];

        int n = epoll_wait(workers[key], events, 64, -1);

        for(int i =  0;i < n;i++){
            SchedulerClient* client = (SchedulerClient*)events[i].data.ptr;
            auto data = std::make_shared<std::vector<char>>(4096);
            ssize_t data_len = recv(client->fd, data->data(), 4096, MSG_DONTWAIT);

            if(data_len == 0) {
                clean(client, key);
                continue;
            }
            else if (data_len == -1) {
                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    i++;
                    continue;
                }
                clean(client, key);
            }
            else {
                data->resize(data_len);
                Request req(client->fd,data->data(),data_len);
                client->handler->onRequest(&req);
                i++;
            }
        }
    }
}

void Socket::Scheduler::addClient(Socket::SchedulerClient client){
     auto in = std::min_element(workerPool.begin(),workerPool.end());
     int smallest = std::distance(workerPool.begin(),in);
     SchedulerClient* c = new SchedulerClient;
     c->fd = client.fd;
     c->handler = client.handler;

     epoll_event ev = {};
     ev.events = EPOLLIN | EPOLLET;
     ev.data.ptr = c;

     epoll_ctl(workers[smallest], EPOLL_CTL_ADD, client.fd , &ev);
     workerPool[smallest]++;

}