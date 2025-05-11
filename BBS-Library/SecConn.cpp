#include "pch.h"
#include "SecConn.h"

#include "Crypto.h"

using Crypto::AES::AES;
using Crypto::SHA256::SHA256;

SecConn::SecConn(SocketW socket) {
	this->socket = socket;
	this->aes = AES(Crypto::AES::AESKeyLength::AES_256);
	memset(this->aes_key, 0, sizeof(this->aes_key));
	memset(this->aes_vector, 0, sizeof(this->aes_vector));
	memset(this->cipher_block, 0, sizeof(this->cipher_block));
	this->cipher_counter = 16;
	this->buffer = (uint8_t*)malloc(131072);
}

SecConn::~SecConn() {
	free(this->buffer);
}

int SecConn::ecdh_handshake() {
	// Generate private key and public key
	uint8_t prikey[Crypto::ECDH::PRIVATE_KEY_SIZE];
	uint8_t pubkey[Crypto::ECDH::PUBLIC_KEY_SIZE];
	uint8_t other_pubkey[Crypto::ECDH::PUBLIC_KEY_SIZE];
	uint8_t secret[Crypto::ECDH::PUBLIC_KEY_SIZE];

	if (Crypto::Random::generate(prikey, sizeof(prikey)) != 0) {
		return SecConnStatus::SECONN_CRYPTO_ERR;
	}
	Crypto::ECDH::ecdh_generate_keys(pubkey, prikey);

	if (this->socket.sendAll(pubkey, sizeof(pubkey)) != sizeof(pubkey)) {
		return SecConnStatus::SECONN_SOCK_ERR;
	};

	if (this->socket.recvAll(other_pubkey, sizeof(other_pubkey)) != sizeof(other_pubkey)) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}

	Crypto::ECDH::ecdh_shared_secret(prikey, other_pubkey, secret);

	// hash secret to AES key
	SHA256 sha;
	sha.update(secret, sizeof(secret));
	auto digest = sha.digest();
	memcpy(this->aes_key, digest.data(), sizeof(aes_key));
	aes.KeyExpansion(this->aes_key, this->aes_roundkey);

	return SecConnStatus::SECONN_OK;
}

int SecConn::client_handshake() {
	int status = this->ecdh_handshake();
	if (status != SecConnStatus::SECONN_OK) {
		return status;
	}
	// Receive Initialize Vector (Nonce)
	if (this->socket.recvAll(aes_vector, sizeof(aes_vector)) != sizeof(aes_vector)) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}
	this->cipher_counter = 16;

	return SecConnStatus::SECONN_OK;
}

int SecConn::server_handshake() {
	int status = this->ecdh_handshake();
	if (status != SecConnStatus::SECONN_OK) {
		return status;
	}
	// Generate Initialize Vector (Nonce)
	if (Crypto::Random::generate(aes_vector, sizeof(aes_vector)) != 0) {
		return SecConnStatus::SECONN_CRYPTO_ERR;
	}
	if (this->socket.sendAll(aes_vector, sizeof(aes_vector)) != sizeof(aes_vector)) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}
	this->cipher_counter = 16;

	return SecConnStatus::SECONN_OK;
}

uint8_t SecConn::new_cipher_byte() {
	if (this->cipher_counter >= sizeof(this->cipher_block)) {
		// Increment aes_vector (counter) in CTR mode
		for (int i = 0; i < sizeof(aes_vector); i++) {
			if (aes_vector[i] != 255) {
				aes_vector[i]++;
				break;
			}
			else {
				aes_vector[i] = 0;
			}
		}
		// Generate a new cipher block
		aes.EncryptBlock(aes_vector, cipher_block, aes_roundkey);
		
		// Reset cipher_counter
		cipher_counter = 0;
	}
	return this->cipher_block[this->cipher_counter++];
}

int SecConn::receive_packet(vector<uint8_t>& packet) {
	// Receive Header
	int status;
	status = socket.recvAll(buffer, 2);
	if (status != 2) {
		return SecConnStatus::SECONN_SOCK_ERR;
	}
	buffer[0] = buffer[0] ^ new_cipher_byte();
	buffer[1] = buffer[1] ^ new_cipher_byte();
	int length = buffer[0] * 256 + buffer[1];
	
	// Receive Packet Body
	status = socket.recvAll(buffer, length);
	if (status != length) {
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
	// SHA256 check
	SHA256 sha;
	sha.update(packet.data(), packet.size());
	auto digest = sha.digest();
	if (memcmp(digest.data(), buffer, 32) != 0) {
		return SecConnStatus::SECONN_INVALID_MESSAGE;
	}
	
	return SecConnStatus::SECONN_OK;
}

int SecConn::send_packet(vector<uint8_t> packet) {
	size_t length = packet.size();
	if (length >= 65536) {
		return SecConnStatus::SECONN_INVALID_LENGTH;
	}
	// Calculate SHA256
	uint8_t sha256[32];

	SHA256 sha;
	sha.update(packet.data(), packet.size());
	auto digest = sha.digest();
	memcpy(sha256, digest.data(), sizeof(sha256));
	// Add header
	int counter = 0;
	buffer[counter++] = (length >> 8 & 0xFF) ^ new_cipher_byte();
	buffer[counter++] = (length & 0xFF) ^ new_cipher_byte();
	// Add body
	for (int i = 0; i < packet.size(); i++) {
		buffer[counter++] = packet[i] ^ new_cipher_byte();
	}
	// TODO: Add SHA256 check here
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