
#pragma once

#include "hsp/address.h"
#include "hsp/packet.h"
#include <netdb.h>

namespace HSP
{
    class Client
    {
    private:
        int m_socket = -1;
        bool m_connected = false;
    public:
        Client() = default;
    public:
        void Connect(const Address& addr);
    public:
        int Send(const Packet* packet);
        Packet* Recv();
    public:
        void Disconnect(int how);
    };
}

