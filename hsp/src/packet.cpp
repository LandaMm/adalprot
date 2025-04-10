
#include"hsp/packet.h"
#include <netinet/in.h>
#include<iostream>

constexpr uint32_t MAGIC_BYTES = 0xDEADBEEF;

namespace HSP
{
    void Packet::Serialize(std::vector<uint8_t> &buffer)
    {
        uint32_t magic = htonl(MAGIC_BYTES);
        buffer.insert(buffer.end(), (uint8_t*)&magic, (uint8_t*)&magic + 4);

        std::cout << "DEBUG: Packet version: " << (int)version << std::endl;
        std::cout << "DEBUG: Packet flags: " << (int)flags << std::endl;
        buffer.push_back(version);
        buffer.push_back(flags);

        std::string headersEncoded;
        for (auto &[k, v] : headers)
        {
            headersEncoded += k + ":" + v + '\n';
        }
        headersEncoded.push_back('\n');
        std::cout << "DEBUG: Headers (encoded) (" << headersEncoded.size() << "): '" << headersEncoded << "'\n";

        uint16_t hLen = htons(headersEncoded.size());
        buffer.insert(buffer.end(), (uint8_t*)&hLen, (uint8_t*)&hLen + 2);

        std::cout << "DEBUG: Payload Length: " << payload.size() << std::endl;
        uint32_t pLen = htonl(payload.size());
        buffer.insert(buffer.end(), (uint8_t*)&pLen, (uint8_t*)&pLen + 4);

        buffer.insert(buffer.end(), headersEncoded.begin(), headersEncoded.end());
        buffer.insert(buffer.end(), payload.begin(), payload.end());
    }
}

