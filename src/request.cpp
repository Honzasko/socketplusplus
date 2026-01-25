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