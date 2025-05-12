#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

struct ServerOverview
{
	size_t online_count;
	// [Group Name, user count]
	std::map<std::string, size_t> groups;
	// [Group Name, dm count]
	std::map<std::string, size_t> dms;
	// Packet transformer
	static ServerOverview fromPacket(std::vector<uint8_t> packet);
	std::vector<uint8_t> toPacket();
};

struct UserList
{
	std::set<std::string> usernames;
	// Packet transformer
	static UserList fromPacket(std::vector<uint8_t> packet);
	std::vector<uint8_t> toPacket();
};

struct ServerMessage
{
	std::string type;
	std::string sender;
	std::string content;

	ServerMessage(std::string type, std::string sender, std::string content);

	static ServerMessage welcomeMessage(std::string message);
	static ServerMessage serverMessage(std::string message);
	static ServerMessage joinMessage(std::string nickname);
	static ServerMessage leaveMessage(std::string nickname);
	static ServerMessage fileHintMessage(std::string sender, std::string message);
	static ServerMessage textMessage(std::string sender, std::string message);
	static ServerMessage historyHintMessage();
	// Packet transformer
	static ServerMessage fromPacket(std::vector<uint8_t> packet);
	std::vector<uint8_t> toPacket();
};

struct ServerFileSend
{
	std::string filename;
	std::vector<uint8_t> block;
	// Packet transformer
	static ServerFileSend fromPacket(std::vector<uint8_t> packet);
	std::vector<uint8_t> toPacket();
};