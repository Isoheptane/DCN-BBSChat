// BBS-Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>

#include "Crypto.h"
#include "SocketW.h"
#include "SecConn.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WinSock::SocketW;
using WinSock::SocketWStatus;

int main()
{
    SocketW client = SocketW();
    if (client.init() != SocketWStatus::SW_OK) {
        printf("Failed to initialize WinSock\n");
        return -1;
    }
    if (client.connect("127.0.0.1", 11451) != SocketWStatus::SW_OK) {
        printf("Failed to connect to server.\n");
        return -1;
    }

    SecConn conn(client);

    conn.client_handshake();

    while (true) {
        printf("Your Message: ");
        string data;
        cin >> data;
        
        vector<uint8_t> buffer;
        for (int i = 0; i < data.size(); i++) {
            buffer.push_back(data.data()[i]);
        }

        if (conn.send_packet(buffer) != SecConnStatus::SECONN_OK) {
            printf("Failed to send packet\n");
            return -1;
        }
    }
    return 0;
}
