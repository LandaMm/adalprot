#pragma once

#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>

namespace HSP
{
    class Address
    {
    private:
        sockaddr_in m_addr;
    public:
        Address(sockaddr_in addr);
        Address();
    public:
        char* ToString();
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
        void Send(void* buf, size_t n, int flags = 0);
    public:
        inline Address GetAddress() const { return m_addr; }
    };

    class Server
    {
    private:
        sockaddr *m_addr;
        int m_server;
    public:
        Server(sockaddr* hints);
    public:
        void Bind();
        void Listen(int n);
        Connection Accept();
    public:
        void Close();
        void Shutdown(int how);
    };
}

