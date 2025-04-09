
#include"hsp/client.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

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

    char data[] = "Hello, World!";
    int bytes_sent = client.Send(data, strlen(data));
    std::cout << "INFO: Sent " << bytes_sent << " bytes" << std::endl;

    char buf[1024];
    int bytes_read = client.Recv(buf, 1024);
    std::cout << "INFO: Received " << bytes_read << " bytes" << std::endl;
    std::cout << "INFO: Data: '" << buf << "'" << std::endl;

    client.Close();
    client.Shutdown(SHUT_RDWR);

    return 0;
}

