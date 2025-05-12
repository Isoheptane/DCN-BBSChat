#include <iostream>
#include <string>
#include <thread>
#include <fstream>

#include "Crypto.h"
#include "SocketW.h"

#include "listener.h"
#include "server.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::thread;

using WinSock::SocketW;
using WinSock::SocketWStatus;

int hex_to_int(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	else if (ch >= 'A' && ch <= 'F') {
		return ch - 'a' + 10;
	}
	return 0;
}

int main(int argc, char** argv)
{
	// Chatroom basic initialization
	std::ifstream user_file = std::ifstream("./users.txt");
	int user_counter = 0;
	string username, password;
	while (user_file >> username >> password) {
		std::vector<uint8_t> password_bin;
		for (size_t i = 0; 2 * i + 1 < password.size(); i++) {
			uint8_t value = (hex_to_int(password[2 * i]) << 4) + hex_to_int(password[2 * i + 1]);
			password_bin.push_back(value);
		}
		global_server.new_user(username, password_bin);
		user_counter++;
	}
	user_file.close();
	printf("Loaded %d users.\n", user_counter);

	std::ifstream group_file = std::ifstream("./groups.txt");
	int group_counter = 0;
	string group_name;
	while (group_file >> group_name) {
		global_server.new_group(group_name);
		group_counter++;
	}
	group_file.close();
	printf("Loaded %d groups.\n", group_counter);

		
	
	// Create listener
	SocketW listener = SocketW();
	listener.init();

	// Prepare bind address
	string bindAddress = string("0.0.0.0");
	uint16_t port = 11451;
	if (argc == 3) {
		bindAddress = string(argv[1]);
		port = atoi(argv[2]);
	}
	else if (argc == 2) {
		port = atoi(argv[1]);
	}

	// Bind and listen
	printf("Using bind address %s:%hu\n", bindAddress.c_str(), port);
	if (listener.bind(bindAddress.c_str(), port) != SocketWStatus::SW_OK) {
		printf("Failed to bind socket to address.\n");
		return 1;
	}

	if (listener.listen(SOMAXCONN) != SocketWStatus::SW_OK) {
		printf("Failed to start listening.\n");
		return 2;
	}

	// Start listener on another thread
	std::thread listenerThread(startListener, listener);
	listenerThread.detach();

	while (true) {
		// Other tasks
		global_server.file_pool.closeAllTimedout(10);
	}
}