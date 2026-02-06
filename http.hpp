#pragma once
#include <RequestHandler.hpp>
#include <string>

class Http : public RequestHandler {
    private:
        std::string html;
    public:
        Http();
        void onRequest(Socket::Request* req);
};
