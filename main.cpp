
#include<iostream>
#include<cstring>
#include<netdb.h>
#include<sys/socket.h>

#include "hsp/reader.h"
#include"hsp/server.h"

int main()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    HSP::Server server = HSP::Server("localhost", "4445", &hints);
    server.Bind();
    server.Listen(5);

    std::cout << "Listening on " << server.GetAddr().ToString() << ":4445" << std::endl;

    while(1) {
        auto conn = server.Accept();
        auto addr = conn.GetAddress();
        std::cout << "INFO: Received connection from: " << addr.ToString() << std::endl;

        HSP::Reader reader = HSP::Reader(1024);
        auto buffer = reader.NewBuffer();

        int bytes_read = conn.Recv(buffer, 1024);
        if (bytes_read > 0)
        {
            std::cout << "INFO: Received " << bytes_read << " bytes from connection" << std::endl;

            reader.ReadBuffer(buffer, bytes_read);
            reader.FreeBuffer(buffer);

            // TODO: read packet till the end
            auto packet = reader.ReadPacket();
            if (!packet)
            {
                std::cerr << "ERROR: Failed to decode packet" << std::endl;
                char msg[] = "Failed to read packet";
                conn.Send(msg, strlen(msg));
            }
            else
            {
                std::cout << "INFO: New packet received:" << std::endl;
                std::cout << "Version:\t" << (int)packet->version << std::endl
                          << "Flags:  \t" << (int)packet->flags << std::endl
                          << "Header Length:\t" << packet->headers.size() << std::endl
                          << "Payload Length:\t" << packet->payload.size() << std::endl;

                std::cout << "[INFO]: Headers" << std::endl;
                for (auto &[k, v] : packet->headers)
                {
                    std::cout << "'" << k << "'" << ": '" << v << "'\n";
                }

                if (packet->headers.find("Content-Encoding") != packet->headers.end())
                {
                    std::string contentType = packet->headers.at("Content-Encoding");
                    if (contentType == "utf-8")
                    {
                        std::cout << "INFO: UTF-8 Encoded Payload:" << std::endl;
                        char* msg = reinterpret_cast<char*>(packet->payload.data());
                        std::cout << "'" << msg << "'" << std::endl;
                    }
                }

                std::vector<uint8_t> data;
                packet->Serialize(data);
                conn.Send(data.data(), data.size());
            }
        } else {
            reader.FreeBuffer(buffer);
        }

        std::cout << "INFO: Closing connection: " << addr.ToString() << std::endl;
        conn.Close();
        conn.Shutdown(SHUT_RDWR);
    }

    server.Close();
    server.Shutdown(SHUT_RDWR);

    return 0;
}

