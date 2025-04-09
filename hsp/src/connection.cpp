
#include<stdlib.h>
#include<cstring>
#include<errno.h>
#include<iostream>
#include<unistd.h>
#include"hsp/connection.h"

namespace HSP
{
    Connection::Connection(int socket) : m_addr(nullptr)
    {
        sockaddr *addr = (sockaddr*)malloc(sizeof(sockaddr));
        m_addr = Address(addr);
        socklen_t addrlen = sizeof(*addr);
        if (getpeername(socket, (sockaddr*)addr, &addrlen) == -1)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to get peer name: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        m_socket = socket;
    }

    void Connection::Close()
    {
        close(m_socket);
    }
    
    void Connection::Shutdown(int how)
    {
        shutdown(m_socket, how);
    }

    int Connection::Recv(void* buf, size_t n, int flags)
    {
        return recv(m_socket, buf, n, flags);
    }

    int Connection::Send(void* buf, size_t n, int flags)
    {
        return send(m_socket, buf, n, flags);
    }
}

