
#include"hsp/router.h"
#include "hsp/header.h"
#include <iostream>

namespace HSP
{
    Response* Router::Listener(Request* req)
    {
        std::string route = req->GetHeader(H_ROUTE);
        std::cout<<"[ROUTER] Got new request to '" << route << "'\n";
        if (!route.empty())
        {
            if (m_routes.find(route) != m_routes.end())
            {
                return m_routes.at(route)(req);
            }
        }
        
        // Unknown Route
        Response* res = new Response;
        res->AddHeader(std::make_pair(H_STATUS, std::to_string(S_LOST)));
        return res;
    }

    Router::Router(Server* server)
    {
        server->SetListener([this](Request* req) { return this->Listener(req); });
    }

    void Router::DeleteRoute(std::string route)
    {
        m_routes.erase(route);
    }

    void Router::AddRoute(std::string path, Route route)
    {
        m_routes.insert(std::make_pair(path, route));
    }
}

