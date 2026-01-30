#include "http.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "include/request.hpp"

void Http::onRequest(Socket::Request* req){
    std::vector<char> data = req->getRawData();
    std::string header(data.begin(),data.end());
    std::cout << header << std::endl;
    std::string html = "Hello,http!";
    std::string http = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8 \r\n"
    "Content-Length:" + std::to_string(html.size()) +"\r\n"
    "Connection: close\r\n"
    "\r\n";

    *req  << http + html;
}