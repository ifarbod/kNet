// EndPoint.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CTNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file EndPoint.h
    @brief The class \ref kNet::EndPoint Endpoint. Represents an endpoint of a network connection. */

#ifndef _WIN32
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include <cstdio>
#include <cstring>
#include <string>

namespace kNet
{

/// EndPoint represents a (ip, port) source or destination of a socket.
struct EndPoint
{
    /// The IPv4 address of the endpoint, stored in big-endian. ///\todo Not IPv6-capable.
    unsigned char ip[4];

    unsigned short port;

    EndPoint() { Reset(); }

    /// Clears all fields of this structure to zero.
    void Reset() { memset(this, 0, sizeof(EndPoint)); }

    ///\todo Not IPv6-capable.
    bool operator<(const EndPoint& rhs) const
    {
        if (ip[0] < rhs.ip[0])
            return true;
        if (ip[0] > rhs.ip[0])
            return false;
        if (ip[1] < rhs.ip[1])
            return true;
        if (ip[1] > rhs.ip[1])
            return false;
        if (ip[2] < rhs.ip[2])
            return true;
        if (ip[2] > rhs.ip[2])
            return false;
        if (ip[3] < rhs.ip[3])
            return true;
        if (ip[3] > rhs.ip[3])
            return false;
        if (port < rhs.port)
            return true;
        if (port > rhs.port)
            return false;

        return false;
    }

    ///\todo Not IPv6-capable.
    static EndPoint FromSockAddrIn(const sockaddr_in& addr)
    {
        EndPoint endPoint;
#ifdef _WIN32
        endPoint.ip[0] = addr.sin_addr.S_un.S_un_b.s_b1;
        endPoint.ip[1] = addr.sin_addr.S_un.S_un_b.s_b2;
        endPoint.ip[2] = addr.sin_addr.S_un.S_un_b.s_b3;
        endPoint.ip[3] = addr.sin_addr.S_un.S_un_b.s_b4;
#else
        endPoint.ip[0] = addr.sin_addr.s_addr & 0xFF;
        endPoint.ip[1] = (addr.sin_addr.s_addr >> 8) & 0xFF;
        endPoint.ip[2] = (addr.sin_addr.s_addr >> 16) & 0xFF;
        endPoint.ip[3] = (addr.sin_addr.s_addr >> 24) & 0xFF;
#endif
        endPoint.port = ntohs(addr.sin_port);

        return endPoint;
    }

    ///\todo Not IPv6-capable.
    sockaddr_in ToSockAddrIn() const
    {
        using namespace std;

        sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
#ifdef _WIN32
        address.sin_addr.S_un.S_un_b.s_b1 = ip[0];
        address.sin_addr.S_un.S_un_b.s_b2 = ip[1];
        address.sin_addr.S_un.S_un_b.s_b3 = ip[2];
        address.sin_addr.S_un.S_un_b.s_b4 = ip[3];
#else
        address.sin_addr.s_addr = ip[0] | (ip[1] << 8) | (ip[2] << 16) | (ip[3] << 24);
#endif

        return address;
    }

    ///\todo Not IPv6-capable.
    std::string IPToString() const
    {
        char str[256];
        sprintf(str, "%d.%d.%d.%d", (unsigned int)ip[0], (unsigned int)ip[1], (unsigned int)ip[2], (unsigned int)ip[3]);
        return std::string(str);
    }

    ///\todo Not IPv6-capable.
    std::string ToString() const
    {
        char str[256];
        sprintf(str, "%d.%d.%d.%d:%d", (unsigned int)ip[0], (unsigned int)ip[1], (unsigned int)ip[2],
            (unsigned int)ip[3], (unsigned int)port);
        return std::string(str);
    }
};

}  // ~kNet
