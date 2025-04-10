
#pragma once
#include <functional>
#include<netdb.h>

#include "hsp/address.h"
#include "hsp/connection.h"
#include "hsp/request.h"
#include "hsp/response.h"

#define SERVER_RECV_BUFFER_SIZE 1024
#define SERVER_BACKLOG 10

namespace HSP
{
    class Server
    {
    public:
        using Listener = std::function<Response*(Request*)>;
    private:
        Address m_addr;
        int m_server;
        Listener m_listener;
    public:
        Server(const char* host, const char* port, const addrinfo* hints);
    private:
        void Bind();
        void Listen(int n);
        Connection Accept();
    private:
        void Close();
        void Shutdown(int how);
    public:
        inline Address GetAddr() const { return m_addr; }
    public:
        void Stop();
        void Start(Listener listener);
    };
}

