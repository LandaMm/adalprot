
#pragma once

#include "hsp/connection.h"
#include "hsp/packet.h"
#include <cstdint>

namespace HSP
{
    class Request
    {
    private:
        Packet* m_packet;
        Connection* m_connection;
    public:
        Request(Packet* packet, Connection* conn)
            : m_packet(packet), m_connection(conn) { }
        ~Request();
    public:
        inline const Connection* GetConnection() const { return m_connection; }
    public:
        std::string GetHeader(std::string headerName) const;
        void ReadPayload(std::vector<uint8_t>& buffer) const;
    };
}

