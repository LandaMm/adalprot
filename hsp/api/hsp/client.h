
#pragma once

#include "hsp/address.h"
#include <netdb.h>

namespace HSP
{
    class Client
    {
    private:
        int m_socket;
        bool m_connected;
        Address m_addr;
    public:
        Client(const addrinfo* addr);
    public:
        void Connect();
        int Send(const void *buf, size_t n, int flags = 0);
        int Recv(void *buf, size_t n, int flags = 0);
    public:
        void Close();
        void Shutdown(int how);
    };
}

