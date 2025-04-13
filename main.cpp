
#include <csignal>
#include <fstream>
#include<iostream>
#include<cstring>
#include<netdb.h>
#include<sys/socket.h>

#include "hsp/address.h"
#include"hsp/request.h"
#include"hsp/response.h"
#include "hsp/router.h"
#include"hsp/server.h"

HSP::Response* PongRoute(HSP::Request* req)
{
    std::cout << "[MAIN] New Pong from '" << req->GetConnection()->GetAddress().ToString() << "'" << std::endl;

    std::vector<uint8_t> data;
    req->ReadPayload(data);
    std::cout << "[MAIN] Received payload with size " << data.size() << std::endl;

    char* msg = reinterpret_cast<char*>(data.data());
    std::cout << "[MAIN] Payload: '" << msg << "'\n";

    HSP::Response *res = new HSP::Response();
    res->AddHeader(std::pair("Content-Encoding", "utf-8"));
    res->AddHeader(std::pair("Ping", "pong"));
    res->InsertData(data.begin(), data.end());

    return res;
}

HSP::Response* FileRoute(HSP::Request* req)
{
    std::cout << "[MAIN] File Upload Request from '"
        << req->GetConnection()->GetAddress().ToString() << std::endl;

    std::vector<uint8_t> data;
    req->ReadPayload(data);

    std::cout << "[MAIN] File size is " << data.size() << " bytes" << std::endl;

    std::string fileName = req->GetHeader("Filename");
    if (fileName.empty()) fileName = "outfile";
    fileName = "server_" + fileName;

    std::ofstream out(fileName);

    if (!out)
    {
        std::cout << "[MAIN] Failed to open file stream" << std::endl;
    }

    out.write(reinterpret_cast<char*>(data.data()), data.size());

    HSP::Response* res = new HSP::Response;
    res->AddHeader(std::make_pair("Filename", fileName));
    res->AddHeader(std::make_pair("Content-Encoding", "utf-8"));
    char msg[] = "Hello, World!";
    res->InsertData((uint8_t*)msg, (uint8_t*)msg + strlen(msg));
    return res;
}

HSP::Server* server;

void handle_signal(int signal)
{
    if (signal == SIGINT && server)
    {
        server->Stop();
    }
}

int main()
{
    server = new HSP::Server(HSP::Address::FromString("127.0.0.1:3000", AF_INET));

    std::cout << "Listening on " << server->GetAddr().ToString() << std::endl;

    HSP::Router router = HSP::Router(server);

    router.AddRoute("/pong", PongRoute);
    router.AddRoute("/file-upload", FileRoute);

    std::signal(SIGINT, handle_signal);

    server->Start();

    std::cout << "[MAIN] Server closed" << std::endl;

    return 0;
}

