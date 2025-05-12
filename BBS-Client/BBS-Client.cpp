// BBS-Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>

#include "Crypto.h"
#include "SocketW.h"
#include "SecConn.h"
#include "Commands.h"

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
		int available = conn.available(10000);
		if (available < 0) {
			printf("Failed to check availability (Error %d), disconnecting...\n", WSAGetLastError());
			conn.disconnect();
			break;
		}
		if (available > 0) {
			// Process packets
			while (conn.available(0)) {
				vector<uint8_t> packet;
				int status = conn.receive_packet(packet);
				if (status != SecConnStatus::SECONN_OK) {
					printf("Failed to receive packet (Error %d, %d), disconnecting...\n", status, WSAGetLastError());
					// Maybe close the connection
					conn.disconnect();
					break;
				}
				string packetType = getPacketCommand(packet);
				printf("Received command %s packet\n", packetType.c_str());
			}
		}
		else {
			// Write operations
		}
    }
    return 0;
}
