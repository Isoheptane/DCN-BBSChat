#include "pch.h"
#include "Buffer.hpp"
#include "Commands.h"

using std::string;
using std::vector;

std::string getPacketCommand(const std::vector<uint8_t>& packet) {
	size_t length = take_uint8(packet, 0);
	return take_string(packet, 1, length);
}

std::vector<uint8_t> makeBodylessPacket(string command) {
	std::vector<uint8_t> buffer;
	append_uint8(buffer, command.size());
	append_vector(buffer, command);
	return buffer;
}