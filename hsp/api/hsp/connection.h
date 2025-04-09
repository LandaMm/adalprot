
#pragma once
#include"hsp/address.h"

namespace HSP
{
    class Connection
    {
    private:
        int m_socket;
        Address m_addr;
    public:
        Connection(int socket);
    public:
        void Close();
        void Shutdown(int how);
    public:
        int Recv(void* buf, size_t n, int flags = 0);
        int Send(void* buf, size_t n, int flags = 0);
    public:
        inline Address GetAddress() const { return m_addr; }
    };
}

