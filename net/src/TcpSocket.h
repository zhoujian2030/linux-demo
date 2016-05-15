/*
 * TcpSocket.h
 *
 *  Created on: May 09, 2016
 *      Author: z.j
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "Socket.h"

namespace net {

    // This TcpSocket object can be created by TCP server as a new accepted TCP socket, 
    // or created as a TCP client socket in TCP client (need to call connect() after created)
    class TcpSocket : public Socket {
    public:
        virtual ~TcpSocket();

    protected:
        // Only TcpServerSocket is allowed to create a TcpSocket with an
        // created(connected) socket fd.
        friend class TcpServerSocket;

        TcpSocket(int socket, Socket::InetAddressPort& theRemoteAddrPort);

        virtual void handleInput(Socket* theSocket);

    private: 
        std::string m_remoteIp;
        unsigned short m_remotePort;
    };
}

#endif