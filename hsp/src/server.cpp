
#include<errno.h>
#include<iostream>
#include<cstring>
#include<unistd.h>
#include"hsp/server.h"

namespace HSP
{
    Server::Server(const char* host, const char* port, const addrinfo* hints) : m_addr(nullptr)
    {
        addrinfo *res;
        if (getaddrinfo(host, port, hints, &res) == -1)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to get addrinfo for server: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        m_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        m_addr = Address(res->ai_addr);
    }

    void Server::Bind()
    {
        if (bind(m_server, m_addr.GetAddr(), m_addr.GetAddrLen()))
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to bind server: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void Server::Listen(int n)
    {
        listen(m_server, n);
    }

    Connection Server::Accept()
    {
        sockaddr_storage clientaddr;
        socklen_t addrlen = sizeof(clientaddr);
        int client = accept(m_server, (sockaddr*)&clientaddr, &addrlen);
        if (client == -1)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to accept connection: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return Connection(client);
    }

    void Server::Close()
    {
        close(m_server);
    }

    void Server::Shutdown(int how)
    {
        shutdown(m_server, how);
    }
}

