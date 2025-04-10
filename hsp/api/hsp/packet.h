
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace HSP
{
    class Packet
    {
    public:
        uint8_t version;
        uint8_t flags;
        std::unordered_map<std::string, std::string> headers;
        std::vector<uint8_t> payload;
    public:
        void Serialize(std::vector<uint8_t> &buffer);
    };
}

