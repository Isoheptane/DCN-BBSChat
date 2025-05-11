#pragma once

#include "SocketW.h"

#include <cstdint>
#include <vector>

using WinSock::SocketW;

using std::vector;

enum SecConnStatus {
	SECONN_OK = 0,
	SECONN_SOCK_ERR = -1,
	SECONN_CRYPTO_ERR = -2,
	SECONN_INVALID_LENGTH = -3
};

class SecConn
{
private:
	SocketW socket;
	uint8_t aes_key[16];
	uint8_t cipher_block[16];
	int cipher_counter;

	uint8_t* buffer;

	uint8_t new_cipher_byte();
public:
	SecConn(SocketW socket);
	int handshake();
	int receive_packet(vector<uint8_t>& packet);
	int send_packet(vector<uint8_t> packet);
	bool connected();
};

