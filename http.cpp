#include "http.hpp"
#include <string>
#include <request.hpp>
#include <fstream>
#include <sstream>

Http::Http(){
    std::ifstream file;
    file.open("index.html");
    if(  file.is_open()){
        std::stringstream buff;
        buff << file.rdbuf();
        html = buff.str();
    }
    else {
        html = "Helo,http!";
    }    


    std::string line;
    while (std::getline(file,line)) {
        html += line; 
    }
    file.close();

 
    std::string http("HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8 \r\n"
    "Content-Length:" + std::to_string(html.size()) +"\r\n"
    "\r\n");

    html = http + html;

}


void Http::onRequest(Socket::Request* req){
    *req << html;
    req->commit();
}