#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>

namespace WinSock {
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
        sockaddr_in sockAddr;
        sockaddr_in peerAddr;

    public:
        SocketW();
        SocketW(SOCKET socket);
        ~SocketW();

        int init();
        int bind(const char* ip, int port);
        int listen(int backlog);
        int accept(SOCKET* clientSock);

        /// <summary>
        /// Use socket as client, connect to a server
        /// </summary>
        /// <param name="serverName">Server Name</param>
        /// <param name="port">Remote Port</param>
        /// <returns></returns>
        int connect(const char* serverName, unsigned short port);

        /// <summary>
        /// Send data.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="length"></param>
        /// <returns>Sent length. -1 if error occurs. 0 if connection closed.</returns>
        int send(const char* buffer, int length);

        /// <summary>
        /// Receive data.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="length"></param>
        /// <returns>Received length. -1 if error occurs. 0 if connection closed.</returns>
        int recv(char* buffer, int length);

        /// <summary>
        /// Send a fixed length of data.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="length"></param>
        /// <returns>Return 0 if no error occurs. -1 if error occurs.</returns>
        int sendAll(const char* buffer, int length);

        /// <summary>
        /// Receive a fixed length of data.
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="length"></param>
        /// <returns>Return 0 if no error occurs. -1 if error occurs.</returns>
        int recvAll(char* buffer, int length);
    };
}