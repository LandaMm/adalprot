
#include"hsp.h"
#include<arpa/inet.h>
#include <cerrno>
#include <cstring>
#include<netdb.h>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include<netinet/in.h>
#include<sys/socket.h>

namespace HSP
{
    Address::Address(sockaddr* addr)
    {
        m_addr = addr;
    }

    inline socklen_t Address::GetAddrLen() const
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

