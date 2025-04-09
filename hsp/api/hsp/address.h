
#pragma once
#include<sys/socket.h>

namespace HSP
{
    class Address
    {
    private:
        sockaddr* m_addr;
    public:
        Address(sockaddr* addr);
    public:
        char* ToString() const;
    public:
        inline sockaddr* GetAddr() const { return m_addr; }
        socklen_t GetAddrLen() const;
    };
}

