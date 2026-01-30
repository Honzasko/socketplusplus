#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
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
            void operator<<(std::uint64_t x);
            void operator<<(std::string x);
    };
}