#include "pch.h"
#include "ServerCommands.h"
#include "Buffer.hpp"

using std::string;
using std::vector;

std::string getPacketCommand(const std::vector<uint8_t>& packet) {
	size_t length = take_uint8(packet, 0);
	return take_string(packet, 1, length);
}

/*
*	Server Overview
*/
ServerOverview ServerOverview::fromPacket(std::vector<uint8_t> packet) {

	ServerOverview overview;
	size_t counter = 0;
	size_t cml = take_uint8(packet, counter++);
	counter += cml;

	overview.online_count = take_uint8(packet, counter++);

	size_t groupCount = take_uint8(packet, counter++);

	for (size_t i = 0; i < groupCount; i++) {
		size_t nameLen = take_uint8(packet, counter++);
		string name = take_string(packet, counter, nameLen);
		counter += nameLen;
		size_t online = take_uint8(packet, counter++);

		overview.groups[name] = online;
	}

	size_t dmCount = take_uint8(packet, counter++);

	for (size_t i = 0; i < dmCount; i++) {
		size_t nameLen = take_uint8(packet, counter++);
		string name = take_string(packet, counter, nameLen);
		counter += nameLen;
		size_t count = take_uint8(packet, counter++);

		overview.dms[name] = count;
	}

	return overview;
}

std::vector<uint8_t> ServerOverview::toPacket() {
	std::vector<uint8_t> buffer;

	const static std::string COMMAND = std::string("message");
	append_uint8(buffer, COMMAND.size());
	append_vector(buffer, COMMAND);

	append_uint8(buffer, this->online_count);

	append_uint8(buffer, this->groups.size());
	for (auto it : groups) {
		append_uint8(buffer, it.first.size());
		append_vector(buffer, it.first);
		append_uint8(buffer, it.second);
	}

	append_uint8(buffer, this->dms.size());
	for (auto it : dms) {
		append_uint8(buffer, it.first.size());
		append_vector(buffer, it.first);
		append_uint8(buffer, it.second);
	}

	return buffer;
}

/*
*	User List
*/
UserList UserList::fromPacket(std::vector<uint8_t> packet) {
	UserList list;

	size_t counter = 0;
	size_t cml = take_uint8(packet, counter++);
	counter += cml;

	size_t count = take_uint8(packet, counter++);

	for (size_t i = 0; i < count; i++) {
		size_t len = take_uint8(packet, counter++);
		string name = take_string(packet, counter, len);
		counter += len;

		list.usernames.insert(name);
	}

	return list;
}

std::vector<uint8_t> UserList::toPacket() {
	std::vector<uint8_t> buffer;

	const static std::string COMMAND = std::string("user_list");
	append_uint8(buffer, COMMAND.size());
	append_vector(buffer, COMMAND);

	for (auto name : this->usernames) {
		append_uint8(buffer, name.size());
		append_vector(buffer, name);
	}

	return buffer;
}

/*
*	Server Message
*/
ServerMessage::ServerMessage(string type, string sender, string content)
	: type(type), sender(sender), content(content) {
}

ServerMessage ServerMessage::welcomeMessage(string message) {
	return ServerMessage("welcome", "", message);
}

ServerMessage ServerMessage::serverMessage(string message) {
	return ServerMessage("server", "", message);
}

ServerMessage ServerMessage::joinMessage(string nickname) {
	return ServerMessage("join", nickname, "");
}

ServerMessage ServerMessage::leaveMessage(string nickname) {
	return ServerMessage("leave", nickname, "");
}

ServerMessage ServerMessage::fileHintMessage(string sender, string message) {
	return ServerMessage("file_hint", sender, message);
}

ServerMessage ServerMessage::textMessage(string sender, string message) {
	return ServerMessage("text", sender, message);
}

ServerMessage ServerMessage::historyHintMessage() {
	return ServerMessage("history_hint", "", "");
}

ServerMessage ServerMessage::fromPacket(vector<uint8_t> packet) {
	size_t counter = 0;
	size_t cml = take_uint8(packet, counter++);
	counter += cml;

	size_t typeLen = take_uint8(packet, counter++);
	string type = take_string(packet, counter, typeLen);
	counter += typeLen;

	size_t senderLen = take_uint8(packet, counter++);
	string sender = take_string(packet, counter, senderLen);
	counter += senderLen;

	size_t contentLen = take_uint16(packet, counter);
	counter += 2;
	string content = take_string(packet, counter, contentLen);
	counter += contentLen;

	return ServerMessage(type, sender, content);
}

vector<uint8_t> ServerMessage::toPacket() {
	std::vector<uint8_t> buffer;

	const static std::string COMMAND = std::string("message");
	append_uint8(buffer, COMMAND.size());
	append_vector(buffer, COMMAND);

	append_uint8(buffer, this->type.size());
	append_vector(buffer, this->type);
	append_uint8(buffer, this->sender.size());
	append_vector(buffer, this->sender);
	append_uint16(buffer, this->content.size());
	append_vector(buffer, this->content);

	return buffer;
}

/*
*	Server File Send
*/
ServerFileSend ServerFileSend::fromPacket(std::vector<uint8_t> packet) {
	ServerFileSend send;

	size_t counter = 0;
	size_t cml = take_uint8(packet, counter++);
	counter += cml;

	size_t nameLen = take_uint8(packet, counter++);
	send.filename = take_string(packet, counter, nameLen);
	counter += nameLen;

	size_t blockLen = take_uint16(packet, counter);
	counter += 2;
	send.block = take_bytes(packet, counter, blockLen);

	return send;
}

std::vector<uint8_t> ServerFileSend::toPacket() {
	std::vector<uint8_t> buffer;

	const static std::string COMMAND = std::string("file_send");
	append_uint8(buffer, COMMAND.size());
	append_vector(buffer, COMMAND);

	append_uint8(buffer, this->filename.size());
	append_vector(buffer, this->filename);

	append_uint16(buffer, this->block.size());
	append_vector(buffer, this->block);

	return buffer;
}