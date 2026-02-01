#include <iostream>
#include <string>
#include <vector>
#include <request.hpp>
#include "chat.hpp"

Chat::Chat(){
    first = true;
}

void Chat::onRequest(Socket::Request* req){
    std::vector<char> data = req->getRawData();
    std::string text(data.begin(),data.end());
    
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }

    if(first){
         name = text;
         first  = false;
    }
    else std::cout << name  << ":" << text << std::endl;

}