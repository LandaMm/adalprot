
#include<iostream>
#include<cstring>
#include<netdb.h>
#include<sys/socket.h>

#include "hsp/request.h"
#include "hsp/response.h"
#include"hsp/server.h"

HSP::Response* OnRequest(HSP::Request* req)
{
    std::cout << "[MAIN] New Request from '" << req->GetConnection()->GetAddress().ToString() << "'" << std::endl;
    std::string route = req->GetHeader("Route");
    if (!route.empty())
    {
        std::cout << "[MAIN] Route '" << route << "'\n";
    }
    std::vector<uint8_t> data;
    req->ReadPayload(data);
    std::cout << "[MAIN] Received payload with size " << data.size() << std::endl;

    char* msg = reinterpret_cast<char*>(data.data());
    std::cout << "[MAIN] Payload: '" << msg << "'\n";

    HSP::Response *res = new HSP::Response();
    res->AddHeader(std::pair("Content-Encoding", "utf-8"));
    res->InsertData(data.begin(), data.end());

    return res;
}

int main()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    HSP::Server server = HSP::Server("localhost", "4445", &hints);

    std::cout << "Listening on " << server.GetAddr().ToString() << ":4445" << std::endl;

    server.Start(OnRequest);

    server.Stop();

    return 0;
}

