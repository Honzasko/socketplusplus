#pragma once
#include <thread>
#include <vector>
#include "RequestHandler.hpp"

namespace Socket {

    typedef struct {
        int fd;
        RequestHandler* handler;
    }SchedulerClient;

    class Scheduler {
        private:
            int nproc;
            std::vector<std::thread> threads; 
            std::vector<int> workerPool;
            void task(int key);
            std::vector<int> workers;
            void clean(SchedulerClient* client,int worker);
        public:
            Scheduler();
            void addClient(SchedulerClient client);
    };
}