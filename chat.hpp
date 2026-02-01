#pragma once
#include "RequestHandler.hpp"
#include <string>

class Chat : public RequestHandler {
    private:
        std::string name;
        bool first = false;
    public:
        Chat();
        void onRequest(Socket::Request* req);
};