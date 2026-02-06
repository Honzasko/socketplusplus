#pragma once
#include <thread>
#include <unordered_map>
#include <vector>
#include "server/scheduler.hpp"
#include <string>

namespace Schedulers {

    class Multireactor : public Socket::Scheduler{
        private:
            std::unordered_map<std::string, std::string> cache;
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