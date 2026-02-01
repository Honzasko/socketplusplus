#pragma once
#include <thread>
#include <vector>
#include "RequestHandler.hpp"
#include "server/scheduler.hpp"

namespace Schedulers {

    typedef struct {
        int fd;
        RequestHandler* handler;
    }SchedulerClient;

    class Multireactor : public Socket::Scheduler{
        private:
            int nproc;
            std::vector<std::thread> threads; 
            std::vector<int> workerPool;
            void task(int key);
            std::vector<int> workers;
            void clean(Socket::SchedulerClient* client,int worker);
            void processClient(Socket::SchedulerClient* client,int key);
        public:
            Multireactor();
            void addClient(Socket::SchedulerClient client);
    };
}