
#include <cerrno>
#include<errno.h>
#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<fcntl.h>
#include<thread>
#include<unistd.h>
#include"hsp/server.h"
#include"hsp/connection.h"
#include"hsp/reader.h"
#include"hsp/request.h"

namespace HSP
{
    Server::Server(Address addr) : m_addr(addr)
    {
        std::cout << "[SERVER] DEBUG: Addr family is " << addr.GetAddr()->sa_family
            << std::endl;
        m_server = socket(addr.GetAddr()->sa_family, SOCK_STREAM | SOCK_NONBLOCK, 0);

        // Non blocking
        // int flags = fcntl(m_server, F_SETFL, 0);
        // fcntl(m_server, F_SETFL, flags | O_NONBLOCK);
    }

    Response* Server::DefaultListener(Request* req)
    {
        return new Response;
    }

    void Server::Stop()
    {
        std::cout << "[SERVER] Gracefully shutting down" << std::endl;
        for (auto&t : m_handlers)
        {
            if (t.joinable()) t.join();
        }

        m_running = false;
    }

    void Server::HandleConnection(Connection conn)
    {
        HSP::Reader reader = HSP::Reader(SERVER_RECV_BUFFER_SIZE);
        auto buffer = reader.NewBuffer();

        int bytes_read = conn.Recv(buffer, SERVER_RECV_BUFFER_SIZE);

        if (bytes_read < 0) 
        {
            std::cerr << "[SERVER] ERROR: Failed to read data from connection: "
                << conn.GetAddress().ToString() << std::endl;
        }
        else
        {
            reader.ReadBuffer(buffer, bytes_read);
            reader.FreeBuffer(buffer);

            Packet* packet = reader.ReadPacket();
            if (!packet)
            {
                std::cerr << "[SERVER] ERROR: Failed to read packet from connection: "
                    << conn.GetAddress().ToString() << std::endl;
            }
            else
            {
                Request* req = new Request(packet, &conn);
                Response* res = m_listener(req);
                Packet* resPacket = Packet::FromResponse(res);

                std::vector<uint8_t> resBuffer;
                resPacket->Serialize(resBuffer);
                conn.Send(resBuffer.data(), resBuffer.size());
            }
        }

        conn.Close();
        conn.Shutdown(SHUT_RDWR);
    }

    void Server::Start()
    {
        Bind();
        Listen(SERVER_BACKLOG);

        m_running = true;

        while(m_running)
        {
            Connection *conn = Accept();
            if (conn)
            {
                std::cout << "[SERVER] New connection from '"
                    << conn->GetAddress().ToString()
                    << std::endl;
                m_handlers.emplace_back([this, conn]() { this->HandleConnection(*conn); });
            }            
        }

        Close();
        Shutdown(SHUT_RDWR);
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

    Connection *Server::Accept()
    {
        sockaddr_storage clientaddr;
        socklen_t addrlen = sizeof(clientaddr);
        int client = accept(m_server, (sockaddr*)&clientaddr, &addrlen);
        if (client == -1)
        {
            int errnum = errno;
            if (errnum == EWOULDBLOCK || errno == EAGAIN)
            {
                return nullptr;
            }
            std::cerr << "ERROR: Failed to accept connection: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return new Connection(client);
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

