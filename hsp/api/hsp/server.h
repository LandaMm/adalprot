
#pragma once
#include<netdb.h>

#include "hsp/address.h"
#include "hsp/connection.h"

namespace HSP
{
    class Server
    {
    private:
        Address m_addr;
        int m_server;
    public:
        Server(const char* host, const char* port, const addrinfo* hints);
    public:
        void Bind();
        void Listen(int n);
        Connection Accept();
    public:
        void Close();
        void Shutdown(int how);
    public:
        inline Address GetAddr() const { return m_addr; }
    };
}

