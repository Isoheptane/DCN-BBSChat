// BBS-Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <sstream>

#include "Crypto.h"
#include "SocketW.h"
#include "SecConn.h"
#include "Commands.h"
#include "SetColor.h"

#include "packet_queue.h"
#include "conn_operator.h"

#include "ClientAuthCommands.h"
#include "ClientMessageCommands.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

using WinSock::SocketW;
using WinSock::SocketWStatus;

int main()
{
	string server_addr;
	uint16_t server_port;

	// Prompt welcome and require input data
	printf("Welcome to DCN Chat Client.\n");
	printf("Server address and port: ");
	cin >> server_addr >> server_port;

	printf("Pleasge input [login/reg], your username and password:\n  ");

	string loginType, username, password;
	cin >> loginType >> username >> password;
	if (loginType != "login" && loginType != "reg") {
		printf("Invalid login/reg type. Please input \"login\" or \"reg\"\n");
		return -1;
	}
	Crypto::SHA256::SHA256 sha;
	sha.update(password);
	auto digest = sha.digest();
	std::vector<uint8_t> hash;
	for (uint8_t byte : digest) {
		hash.push_back(byte);
	}

	// Initiate connection
	printf("Initiating connection to server...\n");
    SocketW client = SocketW();
    if (client.init() != SocketWStatus::SW_OK) {
        printf("Failed to initialize WinSock\n");
        return -1;
    }
    if (client.connect(server_addr.c_str(), server_port) != SocketWStatus::SW_OK) {
        printf("Failed to connect to %s:%d.\n", server_addr.c_str(), server_port);
        return -1;
    }

	// Security layer handshake
	printf("Successfully connected to server and initiating security layer handshake...\n");
    SecConn conn(client);
	if (conn.client_handshake() != SecConnStatus::SECONN_OK) {
		printf("Security layer handshake failed.\n");
		return -1;
	}

	printf("Security layer handshake complete.\n");

	// Create queue and add listener thread
	std::shared_ptr<PacketQueue> queue = std::make_shared<PacketQueue>();

	if (loginType == "login") {
		LoginCommand command = LoginCommand{ username, hash };
		queue.get()->push(command.toPacket());
	} else if (loginType == "reg") {
		RegisterCommand command = RegisterCommand{ username, hash };
		queue.get()->push(command.toPacket());
	}

	std::thread op_thread(connection_operator, conn, queue);
	op_thread.detach();

	// Prompt input on main thread
	while (true) {
		string input;

		std::getline(std::cin, input);
		if (input.size() == 0) {
			continue;
		}
		// Command
		if (input[0] == '/') {
			std::stringstream ss;
			ss << input << " INVALID_STRING";
			string command, arg;
			ss >> command >> arg;

			// packet to be sent
			vector<uint8_t> packet;

			if (command == "/overview") {
				makeBodylessPacket("overview");
			}

			queue.get()->push(packet);
			continue;
		}
		// Normal message
		queue.get()->push(ClientMessage::textMessage(input).toPacket());
	}

    return 0;
}