
#pragma once

#include <functional>
#include <unordered_map>

#include "hsp/request.h"
#include "hsp/response.h"
#include "hsp/server.h"

namespace HSP
{
    class Router
    {
    public:
        using Route = std::function<Response*(Request*)>;
    private:
        std::unordered_map<std::string, Route> m_routes;
    public:
        Router(Server* server);
    private:
        Response* Listener(Request* req);
    public:
        void AddRoute(std::string path, Route route);
        void DeleteRoute(std::string path);
    };
}

