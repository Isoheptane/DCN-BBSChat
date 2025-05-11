#include <iostream>
#include <string>

#include "Crypto.h"
#include "SocketW.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

using WinSock::SocketW;

int main()
{
	SocketW listener = SocketW();
	listener.init();
	listener.bind("0.0.0.0", 11451);
	listener.listen(16);

	cout << "Listening.." << endl;

	SocketW client;
	if (listener.accept(&client) != 0) {
		cout << "Failed to accept client" << endl;
		return -1;
	}
	else {
		cout << "Client accepted" << endl;
	}

	while (true) {
		uint16_t length = 0;
		char buffer[4096];
		if (client.recvAll((char*)(&length), 2) == -1) {
			cout << "Failed to receive message head" << endl;
			return -1;
		}
		if (client.recvAll(buffer, length) == -1) {
			cout << "Failed to receive message body" << endl;
			return -1;
		}
		buffer[length] = '\0';
		cout << "Length " << length << " received,\nMessage: " << buffer << endl;
	}
}