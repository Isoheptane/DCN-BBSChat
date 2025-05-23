#include "listener.h"

#include "SecConn.h"
#include "ServerCommands.h"
#include "session.h"
#include "packet_processor.h"
#include "server.h"

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
	printf("[%s:%d] Initiating encryption layer handshake...\n", epAddr.c_str(), epPort);
	conn.server_handshake();
	printf("[%s:%d] Encryption layer handshake success\n", epAddr.c_str(), epPort);

	// Create session
	std::shared_ptr<Session> session = std::make_shared<Session>(epAddr, epPort);
	// session.get()->packet_push(ServerMessage::welcomeMessage("Welcome to this Chatroom.").toPacket());

	// Connection processing loop
	while (conn.connected()) {
		// Check if the connection still need to be completed
		if (session.get()->state == SessionState::STATE_CLOSED && !session.get()->packet_pending()) { 
			break;
		}
		// Non-blocking readibility check
		int available = conn.available(10000);
		if (available < 0) {
			printf("[%s:%d] Failed to check availability (Error %d), disconnecting...\n", epAddr.c_str(), epPort, WSAGetLastError());
			conn.disconnect();
			break;
		}
		if (available > 0) {
			// Process packets
			while (conn.available(0)) {
				vector<uint8_t> packet;
				int status = conn.receive_packet(packet);
				if (status != SecConnStatus::SECONN_OK) {
					printf("[%s:%d] Failed to receive packet (Error %d, %d), disconnecting...\n", epAddr.c_str(), epPort, status, WSAGetLastError());
					// Maybe close the connection
					conn.disconnect();
					break;
				}
				// Process received packet
				packet_processor(session, packet);
			}
		}
		else {
			// Write operations
			while (session.get()->packet_pending()) {
				vector<uint8_t> packet = session.get()->packet_take();
				int status = conn.send_packet(packet);
				if (status != SecConnStatus::SECONN_OK) {
					printf("[%s:%d] Failed to send packet (Error %d, %d), disconnecting...\n", epAddr.c_str(), epPort, status, WSAGetLastError());
					// Maybe close the connection
					conn.disconnect();
					break;
				}
			}
			// Check dm for additional
			check_dm(session);
		}
	}

	global_server.remove_session(session);

	printf("[%s:%d] Client disconnected.\n", epAddr.c_str(), epPort);
}