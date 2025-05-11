#pragma once

#include "SocketW.h"
#include "Crypto.h"

#include <cstdint>
#include <vector>

using WinSock::SocketW;
using Crypto::AES::AES;

using std::vector;

enum SecConnStatus {
	SECONN_OK = 0,
	SECONN_SOCK_ERR = -1,
	SECONN_CRYPTO_ERR = -2,
	SECONN_INVALID_LENGTH = -3,
	SECONN_INVALID_MESSAGE = -4
};

class SecConn
{
private:
	SocketW socket;

	uint8_t* buffer;

	AES aes;
	uint8_t aes_key[32];
	uint8_t aes_roundkey[4 * 4 * 15];
	uint8_t aes_vector[16];
	uint8_t cipher_block[16];
	int cipher_counter;

	uint8_t new_cipher_byte();
	int ecdh_handshake();
public:
	SecConn(SocketW socket);
	~SecConn();
	int server_handshake();
	int client_handshake();
	int receive_packet(vector<uint8_t>& packet);
	int send_packet(vector<uint8_t> packet);
	bool connected();
};

