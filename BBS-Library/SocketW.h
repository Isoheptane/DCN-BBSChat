#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>

namespace WinSock {
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
        sockaddr_in sockAddr;
        sockaddr_in peerAddr;

    public:
        SocketW();
        SocketW(SOCKET socket);
        ~SocketW();

        int init();
        int bind(const char* ip, int port);
        int listen(int backlog);
        int accept(SocketW* sockw);
        int connect(const char* serverName, unsigned short port);
        int send(const char* buffer, int length);
        int recv(char* buffer, int length);
    };
}