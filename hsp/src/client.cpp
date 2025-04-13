
#include"hsp/client.h"
#include "hsp/address.h"
#include "hsp/packet.h"
#include "hsp/reader.h"
#include "hsp/server.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace HSP
{
    void Client::Connect(const Address& addr)
    {
        m_socket = socket(addr.GetAddr()->sa_family, SOCK_STREAM, 0);
        if (m_socket == -1)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to get socket for client: "
                << strerror(errnum) << std::endl;
        }

        if (connect(m_socket, addr.GetAddr(), addr.GetAddrLen()) == -1)
        {
            m_connected = false;
            int errnum = errno;
            std::cerr << "ERROR: Failed to connect to " << addr.ToString()
                << " : " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }

        m_connected = true;
    }

    int Client::Send(const Packet* packet)
    {
        std::vector<uint8_t> buf;
        packet->Serialize(buf);

        int sent = send(m_socket, buf.data(), buf.size(), 0);
        if (sent < 0)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to send " << buf.size() << " bytes: "
                << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }
        return sent;
    }

    Packet* Client::Recv()
    {
        Reader reader = Reader(SERVER_RECV_BUFFER_SIZE);
        auto buf = reader.NewBuffer();

        int recvd = recv(m_socket, buf, SERVER_RECV_BUFFER_SIZE, 0);
        if (recvd < 0)
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to receive " << SERVER_RECV_BUFFER_SIZE << " bytes: "
                << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }

        reader.ReadBuffer(buf, recvd);
        reader.FreeBuffer(buf);

        return reader.ReadPacket();
    }

    void Client::Disconnect(int how)
    {
        close(m_socket);
        shutdown(m_socket, how);
    }
}

