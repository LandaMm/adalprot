
#include"hsp/packet.h"
#include <algorithm>
#include <cctype>
#include <netinet/in.h>

constexpr uint32_t MAGIC_BYTES = 0xDEADBEEF;

namespace HSP
{
    Packet* Packet::FromResponse(const Response* response)
    {
        Packet* packet = new Packet();
        packet->version = PACKET_VERSION;
        packet->flags = 0; // TODO: Support flags depending on response
        
        for (auto &[k, v] : response->GetHeaders())
        {
            packet->headers.insert(std::make_pair(k, v));
        }

        packet->payload.insert(packet->payload.begin(), (uint8_t*)response->RawData(), (uint8_t*)response->RawData() + response->PayloadSize());

        return packet;
    }

    void Packet::Serialize(std::vector<uint8_t> &buffer) const
    {
        uint32_t magic = htonl(MAGIC_BYTES);
        buffer.insert(buffer.end(), (uint8_t*)&magic, (uint8_t*)&magic + 4);

        buffer.push_back(version);
        buffer.push_back(flags);

        std::string headersEncoded;
        for (auto &[k, v] : headers)
        {
            std::string key = k;
            std::transform(key.begin(), key.end(), key.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            headersEncoded += key + ":" + v + '\n';
        }
        headersEncoded.push_back('\n');

        uint16_t hLen = htons(headersEncoded.size());
        buffer.insert(buffer.end(), (uint8_t*)&hLen, (uint8_t*)&hLen + 2);

        uint32_t pLen = htonl(payload.size());
        buffer.insert(buffer.end(), (uint8_t*)&pLen, (uint8_t*)&pLen + 4);

        buffer.insert(buffer.end(), headersEncoded.begin(), headersEncoded.end());
        buffer.insert(buffer.end(), payload.begin(), payload.end());
    }
}

