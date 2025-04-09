
#include"hsp/client.h"
#include "hsp/address.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

namespace HSP
{
    Client::Client(const addrinfo* addr) : m_addr(nullptr)
    {
        m_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        m_addr = Address(addr->ai_addr);
        if (m_socket == -1)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to get socket for client: "
                << strerror(errnum) << std::endl;
        }
    }

    void Client::Connect()
    {
        if (connect(m_socket, m_addr.GetAddr(), m_addr.GetAddrLen()) == -1)
        {
            m_connected = false;
            int errnum = errno;
            std::cerr << "ERROR: Failed to connect to " << m_addr.ToString()
                << " : " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }

        m_connected = true;
    }

    int Client::Send(const void *buf, size_t n, int flags)
    {
        int sent = send(m_socket, buf, n, flags);
        if (sent < 0)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to send " << n << " bytes: "
                << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return sent;
    }

    int Client::Recv(void *buf, size_t n, int flags)
    {
        int recvd = recv(m_socket, buf, n, flags);
        if (recvd < 0)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to receive " << n << " bytes: "
                << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return recvd;
    }

    void Client::Close()
    {
        close(m_socket);
    }

    void Client::Shutdown(int how)
    {
        shutdown(m_socket, how);
    }
}

