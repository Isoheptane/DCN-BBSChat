#include <iostream>
#include <string>
#include <thread>

#include "Crypto.h"
#include "SocketW.h"

#include "listener.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::thread;

using WinSock::SocketW;

int main(int argc, char** argv)
{
	// TODO: Place chatroon initializations here
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
	if (listener.bind(bindAddress.c_str(), 11451) != 0) {
		printf("Failed to bind socket to address.\n");
		return 1;
	}

	if (listener.listen(SOMAXCONN) != 0) {
		printf("Failed to start listening.\n");
		return 2;
	}

	// Start listener on another thread
	std::thread listenerThread(startListener, listener);
	listenerThread.detach();

	while (true) {
		;
	}
}