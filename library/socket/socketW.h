#ifndef SOCKETW_H
#define SOCKETW_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <string.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class SocketW
{
    private:
        SOCKET sock;
        WSAData wsaData;
        struct sockaddr_in serverAddr;

    public:
        SocketW():sock(INVALID_SOCKET) {
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            {
                fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
            }
        }//initializes the socket library
        
        ~SocketW(){
            if (sock != INVALID_SOCKET) {
                closesocket(sock);
            }
            WSACleanup();
        }

        int createsck(){
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock == INVALID_SOCKET) {
                fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
                return -1;
            }
            return 0;
        }

        int bindsck(const char *ip, int port){
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            
            if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
                return -1;
            }
            return 0; // 修复：返回 0 表示成功
        }

        bool listensck(int backlog);



};
#endif