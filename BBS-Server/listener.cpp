#include "listener.h"
#include "SecConn.h"

#include <cstdio>
#include <cstdint>
#include <thread>

using WinSock::SocketWStatus;

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

		printf(
			"Accepted connection from %s:%d\n",
			inet_ntoa(clientSocket.getPeerAddr()->sin_addr),
			ntohs(clientSocket.getPeerAddr()->sin_port)
		);

		printf("Creating new thread for Connection...\n");
		std::thread thread = std::thread(connectionHandler, clientSocket);
		thread.detach();
	}
}

void connectionHandler(SocketW sockw) {
	// TODO: Change to a real connection handler
	SecConn conn(sockw);
	
	// Key exchange handshake
	conn.server_handshake();

	while (conn.connected()) {
		vector<uint8_t> packet;
		int status = conn.receive_packet(packet);
		if (status != SecConnStatus::SECONN_OK) {
			printf("Failed to receive packet (Error %d)\n", status);
			continue;
		}
		// Fake Processor
		packet.push_back('\0');
		printf("Length %d:\n > %s\n", packet.size(), packet.data());
	}
}