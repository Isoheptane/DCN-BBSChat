#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>

#include <cstdint>

namespace WinSock {

    enum SocketWStatus {
        SW_OK = 0,
        SW_ERR = -1,
        SW_RESOLV_ERR = -2,
        SW_WSA_ERR = -3,
    };

    // global WSAData
    extern WSAData g_wsaData;
    extern bool g_wsaInitialized;

    /// Initialize WSA, this will be automatically called if necessary
    bool InitializeWSA();
    /// Deinitialize WSA
    void CleanupWSA();

    class SocketW
    {
    private:
        SOCKET sock;
        bool connected;
        sockaddr_in sockAddr;
        sockaddr_in peerAddr;

    public:
        SocketW();
        ~SocketW();

        int init();
        int bind(const char* ip, int port);
        int listen(int backlog);
        int accept(SocketW* sockW);
        int connect(const char* serverName, unsigned short port);
        int send(const uint8_t* buffer, int length);
        int recv(uint8_t* buffer, int length);
        int sendAll(const uint8_t* buffer, int length);
        int recvAll(uint8_t* buffer, int length);
        void close();
        bool isConnected();
        const sockaddr_in* getSockAddr();
        const sockaddr_in* getPeerAddr();
    };
}