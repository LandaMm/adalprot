
#include"hsp.h"
#include<arpa/inet.h>
#include<iostream>
#include<cstdlib>
#include<netinet/in.h>
#include<sys/socket.h>

namespace HSP
{
    Address::Address(sockaddr_in addr)
    {
        m_addr = addr;
    }

    char* Address::ToString()
    {
        char* p = (char*)malloc(sizeof(char) * INET6_ADDRSTRLEN);
        if (!inet_ntop(m_addr.sin_family, &m_addr, p, INET6_ADDRSTRLEN))
        {
            std::cerr << "ERROR: Failed to convert address to string" << std::endl;
            exit(EXIT_FAILURE);
        }
        return p;
    }

    Connection::Connection(int socket)
    {
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        if (getpeername(socket, (sockaddr*)&addr, &addrlen) == -1)
        {
            std::cerr << "ERROR: Failed to get peer name" << std::endl;
            exit(EXIT_FAILURE);
        }
        m_addr = Address(addr);
        m_socket = socket;
    }
}

