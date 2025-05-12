#include "conn_operator.h"

#include "file_pool.h"

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>
#include <deque>
#include <thread>

#include "Crypto.h"
#include "SocketW.h"
#include "SecConn.h"
#include "Commands.h"
#include "SetColor.h"

using std::vector;
using std::string;

#include "packet_queue.h"
#include "packet_processor.h"

void connection_operator(SecConn conn, std::shared_ptr<PacketQueue> queue) {
	while (true) {
		// Check readability
		int available = conn.available(10000);
		if (available < 0) {
			printf("Failed to check availability (Error %d), disconnecting...\n", WSAGetLastError());
			conn.disconnect();
			break;
		}

		if (available > 0) {
			// Read operations
			while (conn.available(0)) {
				vector<uint8_t> packet;
				int status = conn.receive_packet(packet);
				if (status != SecConnStatus::SECONN_OK) {
					printf("Failed to receive packet (Error %d, %d), disconnecting...\n", status, WSAGetLastError());
					// Maybe close the connection
					conn.disconnect();
					break;
				}
				// Use another handler
				packet_processor(packet);
			}
		}
		else {
			// Write operations every 10ms
			while (queue.get()->available()) {
				vector<uint8_t> packet = queue.get()->take();
				int status = conn.send_packet(packet);
				if (status != SecConnStatus::SECONN_OK) {
					printf("Failed to send packet (Error %d, %d), disconnecting...\n", status, WSAGetLastError());
					// Maybe close the connection
					conn.disconnect();
					break;
				}
			}
			// Other tasks
			file_pool.closeAllTimedout(10);
		}
	}
}