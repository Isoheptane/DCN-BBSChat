#include "pch.h"
#include "SocketW.h"

#include <cstdio>
#include <string.h>

WSAData g_wsaData;
bool g_wsaInitialized = false;


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


void CleanupWSA() {
    if (g_wsaInitialized) {
        WSACleanup();
        g_wsaInitialized = false;
    }
}

SocketW::SocketW() : sock(INVALID_SOCKET) {
    
}

SocketW::~SocketW() {
    // only clean up the socket if it was created
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    
}

int SocketW::createsck() {
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

int SocketW::bindsck(const char *ip, int port) {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int SocketW::listensck(int backlog) {
    if (listen(sock, backlog) == SOCKET_ERROR) {
        fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int SocketW::acceptsck(SOCKET *clientSocket) {
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    SOCKET clientSock = accept(sock, (struct sockaddr*)&clientAddr, &addrLen);
    if (*clientSocket == INVALID_SOCKET) {
        fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return 0; 
}

int SocketW::connectToServer(const char* serverName, unsigned short port) {
    struct hostent* hp;
    if (isalpha(serverName[0])) {
        hp = gethostbyname(serverName);
    } else {
        unsigned int addr = inet_addr(serverName);
        hp = gethostbyaddr((char*)&addr, 4, AF_INET);
    }

    if (hp == NULL) {
        fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
        return -1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    memcpy(&(serverAddr.sin_addr), hp->h_addr, hp->h_length);
    serverAddr.sin_family = hp->h_addrtype;
    serverAddr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return 0;
}

int SocketW::sendsck(const char* buffer, int length) {
    int bytesSent = send(sock, buffer, length, 0);
    if (bytesSent == SOCKET_ERROR) {
        fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
        return -1;
    }
    return bytesSent;
}

int SocketW::recvsck(char* buffer, int length) {
    int bytesRecv = recv(sock, buffer, length, 0);
    if (bytesRecv == SOCKET_ERROR) {
        fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
        return -1;      
    } else if (bytesRecv > 0) {
        buffer[bytesRecv] = '\0'; // Null-terminate the received string
    } else if (bytesRecv == 0) {
        printf("Connection closed\n");
    }
    return bytesRecv;
}
