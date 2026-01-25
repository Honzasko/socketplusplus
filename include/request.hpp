#pragma once

#include <cstddef>
#include <vector>
namespace Socket {
    class Request{
        private:
            int client;
            char*  buffer;
            std::size_t size;
        public:
            Request(int client_fd,char* buffer, std::size_t size);
            std::vector<char> getRawData();
    };
}