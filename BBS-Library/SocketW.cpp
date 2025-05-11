#include "pch.h"

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
                // fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
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

    SocketW::SocketW() : sock(INVALID_SOCKET), connected(false) {
        memset(&sockAddr, 0, sizeof(sockAddr));
        memset(&peerAddr, 0, sizeof(peerAddr));
    }

    SocketW::~SocketW() {

    }

    int SocketW::init() {
        // make sure WSA is initialized before creating a socket
        if (!g_wsaInitialized && !InitializeWSA()) {
            return SocketWStatus::SW_WSA_ERR;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            // fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
            return SocketWStatus::SW_ERR;
        }
        return 0;
    }

    int SocketW::bind(const char* ip, int port) {

        struct hostent* hp;
        unsigned int addr = inet_addr(ip);
        hp = gethostbyaddr((char*)&addr, 4, AF_INET);

        if (hp == NULL) {
            // fprintf(stderr, "Cannot resolve bind address: %d\n", WSAGetLastError());
            return SocketWStatus::SW_RESOLV_ERR;
        }

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        memcpy(&(sockAddr.sin_addr), hp->h_addr, hp->h_length);

        
        if (::bind(this->sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
            // fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
            return SocketWStatus::SW_ERR;
        }
        return 0;
    }

    int SocketW::listen(int backlog) {
        if (::listen(sock, backlog) == SOCKET_ERROR) {
            // fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
            return SocketWStatus::SW_ERR;
        }
        return 0;
    }

    int SocketW::accept(SocketW* sockW) {
        struct sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        *sockW = SocketW();

        sockW->sock = ::accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
        if (sockW->sock == INVALID_SOCKET) {
            // fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
            return SocketWStatus::SW_ERR;
        }
        sockW->connected = true;
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
            // fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
            return SocketWStatus::SW_RESOLV_ERR;
        }

        memset(&peerAddr, 0, sizeof(peerAddr));
        memcpy(&(peerAddr.sin_addr), hp->h_addr, hp->h_length);
        peerAddr.sin_family = hp->h_addrtype;
        peerAddr.sin_port = htons(port);

        if (::connect(sock, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) == SOCKET_ERROR) {
            // fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
            return SocketWStatus::SW_ERR;
        }
        connected = true;
        return 0;
    }

    int SocketW::send(const uint8_t* buffer, int length) {
        int bytesSent = ::send(sock, (const char*)buffer, length, 0);
        if (bytesSent == SOCKET_ERROR) {
            // fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
            connected = false;
            return SocketWStatus::SW_ERR;
        }
        else if (bytesSent == 0) {
            connected = false;
        }
        return bytesSent;
    }

    int SocketW::recv(uint8_t* buffer, int length) {
        int bytesRecv = ::recv(sock, (char*)buffer, length, 0);
        if (bytesRecv == SOCKET_ERROR) {
            // fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
            connected = false;
            return SocketWStatus::SW_ERR;
        }
        else if (bytesRecv == 0) {
            connected = false;
        }
        return bytesRecv;
    }

    int SocketW::sendAll(const uint8_t* buffer, int length) {
        int sent = 0;
        while (sent < length) {
            int sentSize = this->send(buffer, length - sent);
            if (sentSize == SOCKET_ERROR) {
                return SocketWStatus::SW_ERR;
            }
            else if (sentSize == 0) {
                break;
            }
            sent += sentSize;
            buffer += sentSize;
        }
        return sent;
    }

    int SocketW::recvAll(uint8_t* buffer, int length) {
        int received = 0;
        while (received < length) {
            int recvSize = this->recv(buffer, length - received);
            if (recvSize == SOCKET_ERROR) {
                return SocketWStatus::SW_ERR;
            }
            else if (recvSize == 0) {
                break;
            }
            received += recvSize;
            buffer += recvSize;
        }
        return received;
    }

    void SocketW::close() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    }

    bool SocketW::isConnected() {
        return this->connected;
    }

    const sockaddr_in* SocketW::getSockAddr() {
        return &this->sockAddr;
    }

    const sockaddr_in* SocketW::getPeerAddr() {
        return &this->peerAddr;
    }
}

std::string WinSock::getAddressString(const sockaddr_in* addr) {
    char* buffer = (char*)malloc(32);
    if (buffer == NULL) {
        return std::string("");
    }
    inet_ntop(addr->sin_family, &addr->sin_addr, buffer, 32);
    std::string str(buffer);
    free(buffer);
    return str;
}

uint16_t WinSock::getAddressPort(const sockaddr_in* addr) {
    return ntohs(addr->sin_port);
}