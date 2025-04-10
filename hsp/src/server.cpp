
#include<errno.h>
#include<iostream>
#include<cstring>
#include <sys/socket.h>
#include<unistd.h>
#include"hsp/server.h"
#include "hsp/connection.h"
#include "hsp/reader.h"
#include "hsp/request.h"

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

    void Server::Stop()
    {
        Close();
        Shutdown(SHUT_RDWR);
    }

    void Server::Start(Server::Listener listener)
    {
        m_listener = listener;

        Bind();
        Listen(SERVER_BACKLOG);

        while(true)
        {
            Connection conn = Accept();
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

