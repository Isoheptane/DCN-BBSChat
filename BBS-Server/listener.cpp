#include "listener.h"
#include "SecConn.h"

#include <cstdio>
#include <cstdint>
#include <thread>

using WinSock::SocketWStatus;
using std::vector;

void startListener(SocketW listener) {
	printf(
		"Starting listening on %s:%d\n",
		inet_ntoa(listener.getSockAddr()->sin_addr),
		ntohs(listener.getSockAddr()->sin_port)
	);
	// Listen loop
	while (true) {
		SocketW clientSocket;
		if (listener.accept(&clientSocket) != SocketWStatus::SW_OK) {
			printf("Failed to accept connection.\n");
			continue;
		}

		const sockaddr_in* peerAddr = clientSocket.getPeerAddr();

		printf(
			"Accepted connection from %s:%d\n",
			WinSock::getAddressString(peerAddr).c_str(),
			WinSock::getAddressPort(peerAddr)
		);

		// printf("Creating new thread for Connection...\n");
		std::thread thread = std::thread(connectionHandler, clientSocket);
		thread.detach();
	}
}

void connectionHandler(SocketW sockw) {
	const sockaddr_in* peerAddr = sockw.getPeerAddr();
	std::string epAddr = WinSock::getAddressString(peerAddr).c_str();
	uint16_t epPort = WinSock::getAddressPort(peerAddr);

	// Create secured connection upon socket
	SecConn conn(sockw);
	// Key exchange handshake
	printf("Client %s:%d initiating encryption layer handshake...\n", epAddr.c_str(), epPort);
	conn.server_handshake();
	printf("Client %s:%d encryption layer handshake success\n", epAddr.c_str(), epPort);

	// Receive packets loop
	while (conn.connected()) {
		int available = conn.available(10000);
		if (available < 0) {
			printf("Failed to check availability of %s:%d (Error %d), disconnecting...\n", epAddr.c_str(), epPort, WSAGetLastError());
			conn.disconnect();
			break;
		}
		if (available > 0) {
			vector<uint8_t> packet;
			int status = conn.receive_packet(packet);
			if (status != SecConnStatus::SECONN_OK) {
				printf("Failed to receive packet (Error %d), disconnecting...\n", status);
				// Maybe close the connection
				conn.disconnect();
				break;
			}
			// Process received packet
			
			// Dummy Processor
			packet.push_back('\0');
			printf("Length %d:\n > %s\n", packet.size(), packet.data());
		}
		else {
			// Send data when there is no packet to be processed
			printf("NOT AVAILABLE\n");
		}
		Sleep(10);
	}

	printf("Client %s:%d disconnected.\n", epAddr.c_str(), epPort);
}