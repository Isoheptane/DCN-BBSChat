#include "pch.h"
#include "SecConn.h"



SecConn::SecConn(SocketW socket) {
	this->socket = socket;
	memset(this->aes_key, 0, sizeof(this->aes_key));
	memset(this->cipher_block, 0, sizeof(this->cipher_block));
	this->buffer = (uint8_t*)malloc(131072);
}



int SecConn::handshake() {
	// Initialize handshake here
	this->cipher_counter = 0;
	return 0;
}

uint8_t SecConn::new_cipher_byte() {
	if (this->cipher_counter >= sizeof(this->cipher_block)) {
		// TODO: Generate a new cipher block
		cipher_counter = 0;
	}
	return this->cipher_block[this->cipher_counter++];
}

int SecConn::receive_packet(vector<uint8_t>& packet) {
	// Receive Header
	if (socket.recvAll(buffer, 2) != 2) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}
	buffer[0] = buffer[0] ^ new_cipher_byte();
	buffer[1] = buffer[1] ^ new_cipher_byte();
	int length = buffer[0] * 256 + buffer[1];
	
	// Receive Packet Body
	if (socket.recvAll(buffer, length) != length) {
		return SecConnStatus::SECONN_SOCK_ERR;
	};
	packet.clear();
	for (int i = 0; i < length; i++) {
		packet.push_back(buffer[i] ^ new_cipher_byte());
	};

	// Receive SHA-256 checksum
	if (socket.recvAll(buffer, 32) != 32) {
		return SecConnStatus::SECONN_SOCK_ERR;
	};
	for (int i = 0; i < 32; i++) {
		buffer[i] = buffer[i] ^ new_cipher_byte();
	}
	// TODO: Add SHA256 check here
	
	return SecConnStatus::SECONN_OK;
}

int SecConn::send_packet(vector<uint8_t> packet) {
	size_t length = packet.size();
	if (length >= 65536) {
		return SecConnStatus::SECONN_INVALID_LENGTH;
	}
	// Add header
	int counter = 0;
	buffer[counter++] = (length >> 8 & 0xFF) ^ new_cipher_byte();
	buffer[counter++] = (length & 0xFF) ^ new_cipher_byte();
	// Add body
	for (int i = 0; i < packet.size(); i++) {
		buffer[counter++] = packet[i] ^ new_cipher_byte();
	}
	// TODO: Add SHA256 check here
	uint8_t sha256[32];
	memset(sha256, 0, sizeof(sha256));
	for (int i = 0; i < 32; i++) {
		buffer[counter++] = sha256[i] ^ new_cipher_byte();
	}
	// Send packet
	if (socket.sendAll(buffer, counter) != counter) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}
	return 0;
}

bool SecConn::connected() {
	return this->socket.isConnected();
}