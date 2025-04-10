
#include"hsp/client.h"
#include"hsp/packet.h"
#include"hsp/reader.h"
#include<cassert>
#include<cerrno>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<ios>
#include<iostream>
#include<netdb.h>
#include<sys/socket.h>
#include<utility>

using namespace HSP;

int main(int argc, char** argv)
{
    addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo("localhost", "4445", &hints, &res) < 0)
    {
        int errnum = errno;
        std::cerr << "ERROR: Failed to resolve host address: "
            << strerror(errnum) << std::endl;
        return 1;
    }

    Client client = Client(res);
    client.Connect();

    Packet *packet = new Packet();
    packet->version = 1;
    packet->flags = 0;
    packet->headers.insert(std::make_pair("x-protocol", "stream"));
    packet->headers.insert(std::make_pair("Content-Encoding", "utf-8"));
    packet->headers.insert(std::make_pair("Route", "/pong"));
    std::ifstream licenseFile("LICENSE", std::ios::in | std::ios::binary);
    if (!licenseFile)
    {
        std::cerr << "ERROR: Failed to open 'LICENSE' file" << std::endl;
        return 1;
    }
    char data[501] = {0};
    licenseFile.read(data, 500);
    std::streamsize charsRead = licenseFile.gcount();
    std::cout << "DEBUG: Read " << charsRead << " characters from LICENSE\n";
    licenseFile.close();
    packet->payload.insert(packet->payload.end(), data, data + strlen(data));

    std::vector<uint8_t> buffer;
    packet->Serialize(buffer);
    int bytes_sent = client.Send(buffer.data(), buffer.size());
    std::cout << "INFO: Sent " << bytes_sent << " bytes" << std::endl;

    HSP::Reader reader = HSP::Reader(1024);

    auto buf = reader.NewBuffer();
    int bytes_read = client.Recv(buf, 1024);

    std::cout << "INFO: Received " << bytes_read << " bytes" << std::endl;

    if (bytes_read > 0)
    {
        reader.ReadBuffer(buf, bytes_read);
        reader.FreeBuffer(buf);

        auto packet = reader.ReadPacket();

        std::cout << "INFO: New Packet:" << std::endl;
        std::cout << "Version:\t" << (int)packet->version << std::endl;
        std::cout << "Flags:  \t" << (int)packet->version << std::endl;
        std::cout << "Headers:" << std::endl;
        for(auto &[k, v] : packet->headers)
        {
            std::cout << "\t" << '"' << k << '"' << ": " << '"' << v << '"' << std::endl;
        }
        std::cout << "Payload Length:\t" << packet->payload.size() << std::endl;
        if (packet->headers.find("Content-Encoding") != packet->headers.end())
        {
            std::string contentType = packet->headers.at("Content-Encoding");
            if (contentType == "utf-8")
            {
                std::cout << "INFO: UTF-8 Encoded Payload:" << std::endl;
                char* msg = reinterpret_cast<char*>(packet->payload.data());
                std::string msgStr = msg;
                assert(msgStr == data && "DATA LOSS");
                std::cout << "'" << msg << "'" << std::endl;
            }
        }
    }

    client.Close();
    client.Shutdown(SHUT_RDWR);

    return 0;
}

