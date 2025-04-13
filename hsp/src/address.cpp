
#include <cstdio>
#include<iostream>
#include<arpa/inet.h>
#include<cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include"hsp/address.h"

namespace HSP
{
    Address::Address(sockaddr* addr)
    {
        m_addr = addr;
    }

    Address Address::FromString(const char* src, int family)
    {
        std::string ipStr(src);
        std::string ipPart;
        uint16_t port = 0;

        // Try to extract port if present
        size_t portPos;
        if (family == AF_INET6) {
            // IPv6 with port: "[::1]:8080"
            if (ipStr[0] == '[' && (portPos = ipStr.find("]:")) != std::string::npos) {
                ipPart = ipStr.substr(1, portPos - 1);
                port = static_cast<uint16_t>(std::stoi(ipStr.substr(portPos + 2)));
            } else {
                ipPart = ipStr; // No port
            }
        } else {
            // IPv4 with port: "127.0.0.1:8080"
            if ((portPos = ipStr.find(':')) != std::string::npos) {
                ipPart = ipStr.substr(0, portPos);
                port = static_cast<uint16_t>(std::stoi(ipStr.substr(portPos + 1)));
            } else {
                ipPart = ipStr;
            }
        }

        switch(family)
        {
            case AF_INET:
                {
                    sockaddr_in* addr = new sockaddr_in();
                    memset(addr, 0, sizeof(sockaddr_in));
                    addr->sin_family = AF_INET;
                    addr->sin_port = htons(port);

                    int s = inet_pton(AF_INET, ipPart.c_str(), &(addr->sin_addr));
                    if (s <= 0)
                    {
                        std::cerr << "[ADDRESS] ERROR: Failed to parse '"
                            << src << "' as a valid IPv4 address" << std::endl;
                        delete addr;
                        return nullptr;
                    }

                    return Address((sockaddr*)addr);
                }

            case AF_INET6:
                {
                    sockaddr_in6* addr = new sockaddr_in6();
                    memset(addr, 0, sizeof(sockaddr_in6));
                    addr->sin6_family = AF_INET6;
                    addr->sin6_port = htons(port);

                    int s = inet_pton(AF_INET6, ipPart.c_str(), &(addr->sin6_addr));
                    if (s <= 0)
                    {
                        std::cerr << "[ADDRESS] ERROR: Failed to parse '"
                            << src << "' as a valid IPv6 address" << std::endl;
                        delete addr;
                        return nullptr;
                    }

                    return Address((sockaddr*)addr);
                }

            default:
                std::cerr << "[ADDRESS] ERROR: Invalid family provided: "
                    << family << std::endl;
                return nullptr;
        }    }

    socklen_t Address::GetAddrLen() const
    {
        switch (m_addr->sa_family)
        {
        case AF_INET:
        {
            sockaddr_in* addr = (sockaddr_in*)m_addr;
            return sizeof(*addr);
        }
        case AF_INET6:
        {
            sockaddr_in6* addr = (sockaddr_in6*)m_addr;
            return sizeof(*addr);
        }
        default:
            std::cerr << "ERROR: Unsupported addr family: " << m_addr->sa_family << std::endl;
            exit(EXIT_FAILURE);
        }
        return 0;
    }

    char* Address::ToString() const
    {
        char* p = new char[INET6_ADDRSTRLEN + 1];

        void* addr;
        int port;
        switch(m_addr->sa_family)
        {
            case AF_INET:
            {
                sockaddr_in *addr4 = (sockaddr_in*)m_addr;
                addr = &(addr4->sin_addr);
                port = ntohs(addr4->sin_port);
                break;
            }
            case AF_INET6:
            {
                sockaddr_in6 *addr6 = (sockaddr_in6*)m_addr;
                addr = &(addr6->sin6_addr);
                port = ntohs(addr6->sin6_port);
                break;
            }
            default:
            {
                std::cerr << "[ADDRESS] ERROR: Family not supported for stringify: "
                    << m_addr->sa_family << std::endl;
                return nullptr;
            }
        }

        if (!inet_ntop(m_addr->sa_family, addr, p, INET6_ADDRSTRLEN))
        {
            int errnum = errno;
            std::cerr << "ERROR: Failed to convert address to string: " << strerror(errnum) << std::endl;
            exit(EXIT_FAILURE);
        }

        char portStr[5];
        sprintf(portStr, "%d", port);
        return strcat(strcat(p, ":"), portStr);
    }
}

