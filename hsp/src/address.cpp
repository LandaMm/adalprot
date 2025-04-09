
#include<iostream>
#include<arpa/inet.h>
#include<cstring>
#include"hsp.h"

namespace HSP
{
    Address::Address(sockaddr* addr)
    {
        m_addr = addr;
    }

    socklen_t Address::GetAddrLen() const
    {
        switch (m_addr->sa_family)
        {
        case AF_INET:
        {
            sockaddr_in* addr = (sockaddr_in*)m_addr;
            return sizeof(*addr);
        }
        case AF_INET6:
        {
            sockaddr_in6* addr = (sockaddr_in6*)m_addr;
            return sizeof(*addr);
        }
        default:
            std::cerr << "ERROR: Unsupported addr family: " << m_addr->sa_family << std::endl;
            exit(EXIT_FAILURE);
        }
        return 0;
    }

    char* Address::ToString()
    {
        char* p = (char*)malloc(sizeof(char) * INET6_ADDRSTRLEN);
        if (!inet_ntop(m_addr->sa_family, &m_addr, p, INET6_ADDRSTRLEN))
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to convert address to string: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return p;
    }
}

