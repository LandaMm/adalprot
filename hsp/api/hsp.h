#pragma once

#include <netdb.h>

namespace HSP
{
    class Address
    {
    private:
        sockaddr* m_addr;
    public:
        Address(sockaddr* addr);
    public:
        char* ToString();
    public:
        inline sockaddr* GetAddr() const { return m_addr; }
        socklen_t GetAddrLen() const;
    };

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

