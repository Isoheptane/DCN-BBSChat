#include "pch.h"

/*
    I know this is not good, but anyway, this 
    can still be used since we wrapped it in 
    our SocketW class.
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SocketW.h"

#include <cstdio>
#include <string.h>

namespace WinSock {
    WSAData g_wsaData;
    bool g_wsaInitialized = false;

    /// Initialize WSA, this will be automatically called if necessary
    bool InitializeWSA() {
        if (!g_wsaInitialized) {
            if (WSAStartup(MAKEWORD(2, 2), &g_wsaData) != 0) {
                fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
                return false;
            }
            g_wsaInitialized = true;
        }
        return true;
    }
    
    /// Deinitialize WSA
    void CleanupWSA() {
        if (g_wsaInitialized) {
            WSACleanup();
            g_wsaInitialized = false;
        }
    }

    SocketW::SocketW() : sock(INVALID_SOCKET) {
        memset(&sockAddr, 0, sizeof(sockAddr));
        memset(&peerAddr, 0, sizeof(peerAddr));
    }

    SocketW::SocketW(SOCKET socket) : sock(socket) {
        memset(&sockAddr, 0, sizeof(sockAddr));
        memset(&peerAddr, 0, sizeof(peerAddr));
    }

    SocketW::~SocketW() {
        // only clean up the socket if it was created
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    }

    int SocketW::init() {
        // make sure WSA is initialized before creating a socket
        if (!g_wsaInitialized && !InitializeWSA()) {
            return -1;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    int SocketW::bind(const char* ip, int port) {
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY;
        
        if (::bind(this->sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
            fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    int SocketW::listen(int backlog) {
        if (::listen(sock, backlog) == SOCKET_ERROR) {
            fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    int SocketW::accept(SocketW* sockW) {
        struct sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        *sockW = SocketW();

        sockW->sock = ::accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
        if (sockW->sock == INVALID_SOCKET) {
            fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        sockW->peerAddr = clientAddr;

        return 0;
    }

    int SocketW::connect(const char* serverName, unsigned short port) {
        struct hostent* hp;
        if (isalpha(serverName[0])) {
            hp = gethostbyname(serverName);
        }
        else {
            unsigned int addr = inet_addr(serverName);
            hp = gethostbyaddr((char*)&addr, 4, AF_INET);
        }

        if (hp == NULL) {
            fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
            return -1;
        }

        memset(&peerAddr, 0, sizeof(peerAddr));
        memcpy(&(peerAddr.sin_addr), hp->h_addr, hp->h_length);
        peerAddr.sin_family = hp->h_addrtype;
        peerAddr.sin_port = htons(port);

        if (::connect(sock, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) == SOCKET_ERROR) {
            fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return 0;
    }

    int SocketW::send(const char* buffer, int length) {
        int bytesSent = ::send(sock, buffer, length, 0);
        if (bytesSent == SOCKET_ERROR) {
            fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return bytesSent;
    }

    int SocketW::recv(char* buffer, int length) {
        int bytesRecv = ::recv(sock, buffer, length, 0);
        if (bytesRecv == SOCKET_ERROR) {
            fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
            return -1;
        }
        return bytesRecv;
    }

    int SocketW::sendAll(const char* buffer, int length) {
        while (length > 0) {
            int sentSize = this->send(buffer, length);
            if (sentSize == SOCKET_ERROR) {
                return -1;
            }
            length -= sentSize;
            buffer += sentSize;
        }
        return 0;
    }

    int SocketW::recvAll(char* buffer, int length) {
        while (length > 0) {
            int recvSize = this->recv(buffer, length);
            if (recvSize == SOCKET_ERROR) {
                return -1;
            }
            length -= recvSize;
            buffer += recvSize;
        }
        return 0;
    }

}