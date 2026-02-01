#include "../include/request.hpp"
#include <cstddef>
#include <string_view>
#include <sys/socket.h>


Socket::Request::Request(int client_fd,char* buffer, std::size_t size){
    client = client_fd;
    this->buffer = buffer;
    this->size = size;
}

std::vector<char> Socket::Request::getRawData(){
    std::vector<char> data(this->buffer,this->buffer+this->size);
     return data;  
}

void Socket::Request::operator<<(std::uint64_t x){
    std::uint64_t cache = x;
    for(int i = 0;i < 8;i++){
         out_buff.insert(out_buff.end(),cache & 0xFF);
         cache = cache >> 8;
    }

}


void Socket::Request::operator<<(std::string_view x){
    out_buff.insert(out_buff.end(),x.begin(),x.end());
}

void Socket::Request::commit(){
    send(client, out_buff.data(), out_buff.size(), 0);
}

