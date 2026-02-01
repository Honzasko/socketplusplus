#pragma once
#include "request.hpp"

class RequestHandler {

public:
    virtual void onRequest(Socket::Request* req) = 0;
    virtual ~RequestHandler() = default;
};