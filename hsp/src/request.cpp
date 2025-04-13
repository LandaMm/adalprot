
#include"hsp/request.h"
#include <algorithm>

namespace HSP
{
    Request::~Request()
    {
        delete m_packet;
        delete m_connection;
    }

    std::string Request::GetHeader(std::string headerName) const
    {
        std::string key = headerName;
        std::transform(key.begin(), key.end(), key.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (m_packet->headers.find(key) != m_packet->headers.end())
        {
            return m_packet->headers.at(key);
        }
        return std::string("");
    }

    void Request::ReadPayload(std::vector<uint8_t>& buffer) const
    {
        buffer.insert(buffer.end(), m_packet->payload.begin(), m_packet->payload.end());
    }
}

