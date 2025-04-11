
#include"hsp/client.h"
#include"hsp/packet.h"
#include"hsp/reader.h"
#include<cassert>
#include<cerrno>
#include <cstdint>
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
    packet->headers.insert(std::make_pair("Content-Encoding", "utf-8"));
    packet->headers.insert(std::make_pair("Filename", argv[1]));
    packet->headers.insert(std::make_pair("Route", "/file-upload"));
    std::ifstream file(argv[1], std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "ERROR: Failed to open '" << argv[1] << "'" << std::endl;
        return 1;
    }
    uint8_t data[900] = {0};
    file.read(reinterpret_cast<char*>(data), 900);
    std::streamsize bytesRead = file.gcount();
    std::cout << "DEBUG: Read " << bytesRead << " characters from LICENSE\n";
    file.close();
    packet->payload.insert(packet->payload.end(), data, data + bytesRead);

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
    }

    client.Close();
    client.Shutdown(SHUT_RDWR);

    return 0;
}

