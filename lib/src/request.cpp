#include "../include/request.hpp"
#include <cstddef>
#include <sys/socket.h>


Socket::Request::Request(int client_fd,char* buffer, std::size_t size){
    client = client_fd;
    this->buffer = buffer;
    this->size = size;
}

std::vector<char> Socket::Request::getRawData(){
     std::vector<char> data;
    for (size_t i = 0;i < this->size;i++) {
        data.push_back(buffer[i]);
    }
     return data;  
}

void Socket::Request::operator<<(std::uint64_t x){
    char buff[8];
    std::uint64_t cache = x;
    for(int i = 0;i < 8;i++){
         buff[i] = cache & 0xFF;
         cache = cache >> 8;
    }

    send(client, &buff, 8, 0);
}

void Socket::Request::operator<<(std::string x){
    send(client, x.c_str(), x.size(), 0);
}