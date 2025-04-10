
#pragma once
#include"hsp/packet.h"
#include <unordered_map>

namespace HSP
{
    class Reader
    {
    private:
        std::vector<uint8_t> m_buf;
        unsigned int m_bufSize;
    public:
        Reader(unsigned int bufferSize);
        ~Reader() = default;
    public:
        inline void ReadBuffer(const uint8_t* buffer, unsigned int n)
        {
            m_buf.insert(m_buf.end(), buffer, buffer + n);
        }

        inline uint8_t* NewBuffer()
        {
            uint8_t* buffer = new uint8_t[m_bufSize];
            return buffer;
        }

        inline void FreeBuffer(uint8_t* buffer)
        {
            delete buffer;
        }
    public:
        Packet* ReadPacket();
    private:
        std::unordered_map<std::string, std::string> ParseHeaders(const std::string& headerBlock);
        uint32_t ReadUint32(unsigned int& offset);
        uint16_t ReadUint16(unsigned int& offset);
    };
}

