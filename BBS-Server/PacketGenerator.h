#pragma once

#include "message.h"
#include "Buffer.hpp"

#include <string>
#include <cstdint>

class PacketGenerator
{
	static std::vector<uint8_t> fromMessage(Message message) {
		std::vector<uint8_t> buffer;
		const static std::string COMMAND = std::string("message");
		buffer.push_back(COMMAND.size());
		append_vector(buffer, COMMAND);

		buffer.push_back(message.type.size());
		append_vector(buffer, message.type);
		buffer.push_back(message.sender.size());
		append_vector(buffer, message.sender);
		buffer.push_back(message.content.size());
		append_vector(buffer, message.content);

		return buffer;
	}

	static std::vector<uint8_t> fromUserList(std::vector<std::string> users) {
		std::vector<uint8_t> buffer;
		const static std::string COMMAND = std::string("user_list");
		buffer.push_back(COMMAND.size());
		append_vector(buffer, COMMAND);

		for (std::string user : users) {
			buffer.push_back(user.size());
			append_vector(buffer, user);
		}

		return buffer;
	}

	static std::vector<uint8_t> fromFileSend(std::string filename, std::vector<uint8_t> block) {
		std::vector<uint8_t> buffer;
		const static std::string COMMAND = std::string("file_send");
		buffer.push_back(COMMAND.size());
		append_vector(buffer, COMMAND);

		buffer.push_back(filename.size());
		append_vector(buffer, filename);

		append_vector(buffer, block);

		return buffer;
	}
	
};

