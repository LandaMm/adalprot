
#pragma once

#include "hsp/response.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#define PACKET_VERSION 1
#define PACKET_MAX_SUPPORTED_VERSION 1

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
        void Serialize(std::vector<uint8_t> &buffer) const;
    public:
        static Packet* FromResponse(const Response* response);
    };
}

