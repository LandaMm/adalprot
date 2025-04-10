
#include"hsp/reader.h"
#include "hsp/packet.h"
#include<iostream>
#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <vector>

constexpr uint32_t MAGIC_BYTES = 0xDEADBEEF;

namespace HSP
{
    Reader::Reader(unsigned int bufferSize)
    {
        m_bufSize = bufferSize;
        m_buf = std::vector<uint8_t>();
    }

    uint32_t Reader::ReadUint32(unsigned int &offset)
    {
        uint32_t val;
        std::memcpy(&val, &m_buf[offset], 4);
        offset += 4;
        return ntohl(val);
    }

    uint16_t Reader::ReadUint16(unsigned int &offset)
    {
        uint16_t val;
        std::memcpy(&val, &m_buf[offset], 2);
        offset += 2;
        return ntohs(val);
    }

    Packet* Reader::ReadPacket()
    {
        Packet* packet = new Packet();

        if (m_buf.size() < 12)
        {
            std::cerr << "ERROR: Packet is too small" << std::endl;
            return nullptr;
        }
        
        unsigned int offset = 0;

        uint32_t magic = ReadUint32(offset);

        if (magic != MAGIC_BYTES)
        {
            std::cerr << "ERROR: Packet magic is wrong" << std::endl;
            return nullptr;
        }

        packet->version = m_buf[offset++];
        packet->flags = m_buf[offset++];
        uint16_t headerLen = ReadUint16(offset);
        uint16_t payloadLen = ReadUint32(offset);

        if (m_buf.size() < offset + headerLen + payloadLen)
        {
            std::cerr << "ERROR: Incomplete packet" << std::endl;
            return nullptr;
        }

        std::string headerBlock(reinterpret_cast<const char*>(&m_buf[offset]), headerLen);
        offset += headerLen;

        packet->headers = ParseHeaders(headerBlock);
        packet->payload.insert(packet->payload.end(), m_buf.begin() + offset, m_buf.begin() + offset + payloadLen);
        
        return packet;
    }

    std::unordered_map<std::string, std::string> Reader::ParseHeaders(const std::string& headerBlock)
    {
        auto headers = std::unordered_map<std::string, std::string>();
        size_t i = 0;
        while (i < headerBlock.size() && headerBlock[i] != '\n')
        {
            std::string key;
            while(headerBlock[i] != ':')
            {
                if (headerBlock[i] != ' ')
                    key.push_back(headerBlock[i]);
                i++;
            }
            i++;
            std::string value;
            while(headerBlock[i] != '\n')
            {
                if (headerBlock[i] != ' ')
                    value.push_back(headerBlock[i]);
                i++;
            }
            i++;
            headers.insert(std::make_pair(key, value));
        }
        return headers;
    }
}

