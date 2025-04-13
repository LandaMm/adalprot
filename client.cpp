
#include"hsp/client.h"
#include "hsp/address.h"
#include "hsp/header.h"
#include"hsp/packet.h"
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
    Client client = Client();
    client.Connect(Address::FromString("127.0.0.1:3000", AF_INET));

    Packet *packet = new Packet();
    packet->version = 1;
    packet->flags = 0;
    packet->headers.insert(std::make_pair(H_DATA_FORMAT, DF_TEXT));
    packet->headers.insert(std::make_pair("Filename", argv[1]));
    packet->headers.insert(std::make_pair(H_ROUTE, "/file-upload"));

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

    int bytes_sent = client.Send(packet);
    std::cout << "INFO: Sent " << bytes_sent << " bytes" << std::endl;

    auto response = client.Recv();

    if (response)
    {
        std::cout << "INFO: New Packet:" << std::endl;
        std::cout << "Version:\t" << (int)response->version << std::endl;
        std::cout << "Flags:  \t" << (int)response->version << std::endl;
        std::cout << "Headers:" << std::endl;
        for(auto &[k, v] : response->headers)
        {
            std::cout << "\t" << '"' << k << '"' << ": " << '"' << v << '"' << std::endl;
        }
        std::cout << "Payload Length:\t" << response->payload.size() << std::endl;
    }

    client.Disconnect(SHUT_RDWR);

    return 0;
}

