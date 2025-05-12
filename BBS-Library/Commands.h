#pragma once

#include <string>
#include <vector>
#include <map>

std::string getPacketCommand(const std::vector<uint8_t>& packet);

struct ServerOverview
{
	size_t online_count;
	// [Group Name, user count]
	std::map<std::string, size_t> groups;
	// [Group Name, dm count]
	std::map<std::string, size_t> dms;

	static ServerOverview fromPacket(std::vector<uint8_t> packet);

	std::vector<uint8_t> toPacket();
};

struct ServerMessage
{
public:
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

	static ServerMessage fromPacket(std::vector<uint8_t> packet);

	std::vector<uint8_t> toPacket();
};

struct OverviewMessage
{

};