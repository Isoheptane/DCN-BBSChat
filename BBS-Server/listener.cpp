#include "listener.h"

#include <cstdio>
#include <cstdint>
#include <thread>

void startListener(SocketW listener) {
	printf(
		"Starting listening on %s:%d\n",
		inet_ntoa(listener.getSockAddr()->sin_addr),
		ntohs(listener.getSockAddr()->sin_port)
	);
	// Listen loop
	while (true) {
		SocketW clientSocket;
		if (listener.accept(&clientSocket) != 0) {
			printf("Failed to accept connection.\n");
			continue;
		}

		printf(
			"Accepted connection from %s:%d\n",
			inet_ntoa(clientSocket.getPeerAddr()->sin_addr),
			ntohs(clientSocket.getPeerAddr()->sin_port)
		);

		printf("Creating new thread for Connection...");
		std::thread thread = std::thread(connectionHandler, clientSocket);
		thread.detach();
	}
}

void connectionHandler(SocketW conn) {
	// TODO: Change to a real connection handler
	uint16_t length = 0;
	char buffer[4096];
	while (conn.isConnected()) {
		conn.recvAll((char*)&length, 2);
		conn.recvAll(buffer, length);
		buffer[length] = '\0';
		printf("Received %d bytes:\n > %s\n", length, buffer);
	}
}