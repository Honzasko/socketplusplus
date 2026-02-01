#include "http.hpp"
#include <string>
#include <request.hpp>

void Http::onRequest(Socket::Request* req){
    std::string_view html = "Hello,http!";
    std::string http("HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8 \r\n"
    "Content-Length:" + std::to_string(html.size()) +"\r\n"
    "\r\n");

    *req  << http;
    *req << html;

    req->commit();
}