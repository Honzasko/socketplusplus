#include <server/scheduler.hpp>
#include <RequestHandler.hpp>
#include <algorithm>
#include <cerrno>
#include <iterator>
#include <sys/types.h>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <chrono>
 #include <server/scheduler/multireactor.hpp>

Schedulers::Multireactor::Multireactor(){
    nproc = std::thread::hardware_concurrency();
    if(nproc < 1) nproc = 1;

    for(int i = 0;i < nproc;i++){
        workers.push_back(epoll_create1(0));
        threads.emplace_back(&Schedulers::Multireactor::task,this,i);
        workerPool.push_back(0);
    }
}

void Schedulers::Multireactor::clean(Socket::SchedulerClient* client,int  worker){
    delete client->handler;
    epoll_ctl(workers[worker], EPOLL_CTL_DEL, client->fd, nullptr);
    close(client->fd);
    delete client;
    workerPool[worker]--;
}

void Schedulers::Multireactor::processClient(Socket::SchedulerClient* client, int key){
    std::vector<char> data;
    data.reserve(4096);
    ssize_t data_len = recv(client->fd, data.data(), 4096, MSG_DONTWAIT);
    if(data_len == 0) {
        clean(client, key);
        return;
    }
    else if (data_len == -1) {
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            return;;
        }
        clean(client, key);
        }
    else {
        data.resize(data_len);
        Socket::Request req(client->fd,data.data(),data_len);
        client->handler->onRequest(&req);
    }
}

void Schedulers::Multireactor::task(int key){
    int max_reqs = 500;
    int counter = max_reqs / 2;

    while (true) {

        epoll_event events[max_reqs];


        int n = epoll_wait(workers[key], events, max_reqs, -1);
        
        auto start = std::chrono::high_resolution_clock::now();
        for(int i =  0;i < n;i++){
            Socket::SchedulerClient* client = (Socket::SchedulerClient*)events[i].data.ptr;
            processClient(client, key);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if(duration.count() > 1000){
            max_reqs += counter;
            counter *= 2;
            max_reqs = max_reqs % 4097;
        }
        else if (duration.count()) {
            max_reqs -= counter;
            counter /= 2; 
        }

    }
}

void Schedulers::Multireactor::addClient(Socket::SchedulerClient client){
     auto in = std::min_element(workerPool.begin(),workerPool.end());
     int smallest = std::distance(workerPool.begin(),in);
     SchedulerClient* c = new SchedulerClient;
     c->fd = client.fd;
     c->handler = client.handler;

     epoll_event ev = {};
     ev.events = EPOLLIN ;
     ev.data.ptr = c;

     epoll_ctl(workers[smallest], EPOLL_CTL_ADD, client.fd , &ev);
     workerPool[smallest]++;

}