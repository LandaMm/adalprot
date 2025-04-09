
#include<iostream>
#include<cstring>
#include<netdb.h>
#include<sys/socket.h>

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
        char data[1024];
        int bytes_read = conn.Recv(data, 1024);
        if (bytes_read > 0)
        {
            std::cout << "INFO: Received " << bytes_read << " bytes from connection: " << data << std::endl;

            conn.Send(data, strlen(data));
        }

        std::cout << "INFO: Closing connection: " << addr.ToString() << std::endl;
        conn.Close();
        conn.Shutdown(SHUT_RDWR);
    }

    server.Close();
    server.Shutdown(SHUT_RDWR);

    std::cout << ("Amir") << std::endl;
    return 0;
}

