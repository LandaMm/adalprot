
#pragma once
#include <atomic>
#include <functional>
#include<netdb.h>
#include <thread>

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
        std::vector<std::thread> m_handlers;
        std::atomic<bool> m_running = std::atomic<bool>(false);
    public:
        Server(const char* host, const char* port, const addrinfo* hints);
    public:
        static Response* DefaultListener(Request* req);
    private:
        void Bind();
        void Listen(int n);
        Connection *Accept();
        void HandleConnection(Connection conn);
    private:
        void Close();
        void Shutdown(int how);
    public:
        inline Address GetAddr() const { return m_addr; }
    public:
        inline void SetListener(Listener listener) { m_listener = listener; };
        void Stop();
        void Start();
    };
}

