
#include"hsp/request.h"

namespace HSP
{
    Request::~Request()
    {
        delete m_packet;
        delete m_connection;
    }

    std::string Request::GetHeader(std::string headerName) const
    {
        if (m_packet->headers.find(headerName) != m_packet->headers.end())
        {
            return m_packet->headers.at(headerName);
        }
        return std::string("");
    }

    void Request::ReadPayload(std::vector<uint8_t>& buffer) const
    {
        buffer.insert(buffer.end(), m_packet->payload.begin(), m_packet->payload.end());
    }
}

