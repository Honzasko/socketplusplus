#pragma once
#include "RequestHandler.hpp"

namespace Socket {

    typedef struct {
        int fd;
        RequestHandler* handler;
    }SchedulerClient;

    class Scheduler {
        public:
            Scheduler() = default;
            virtual ~Scheduler() = default;
            virtual void addClient(SchedulerClient client) = 0;
    };
}