#pragma once

#ifndef SOCKETW_H
#define SOCKETW_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>

// global WSAData
extern WSAData g_wsaData;
extern bool g_wsaInitialized;

// WSA initialization and cleanup functions
bool InitializeWSA();
void CleanupWSA();

class SocketW
{
private:
    SOCKET sock;
    struct sockaddr_in serverAddr;

public:
    SocketW();
    ~SocketW();
    
    int createsck();
    int bindsck(const char* ip, int port);
    int listensck(int backlog);
    int acceptsck(SOCKET* clientSocket);
    int connectToServer(const char* serverName, unsigned short port);
    int sendsck(const char* buffer, int length);
    int recvsck(char* buffer, int length);
};
#endif
