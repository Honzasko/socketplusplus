#pragma once
#include "include/RequestHandler.hpp"
#include <string>

class Http : public RequestHandler {
    public:
        void onRequest(Socket::Request* req);
};
