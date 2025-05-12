#include "packet_processor.h"
#include "Commands.h"
#include "ServerCommands.h"
#include "SetColor.h"

#include <string>

using std::string;
using SetColor::setColor;
using SetColor::genColor;
using SetColor::Color;

#define HANDLE_IF_MATCH(x, handler) if (command == (x)) { \
	handler(packet); \
	return; \
}

void server_message_handler(const std::vector<uint8_t>&packet);
void user_list_handler(const std::vector<uint8_t>&packet);
void overview_handler(const std::vector<uint8_t>&packet);

void packet_processor(const std::vector<uint8_t>& packet) {
	std::string command = getPacketCommand(packet);

	// printf("Received command %s\n", command.c_str());
	HANDLE_IF_MATCH("message", server_message_handler);
	HANDLE_IF_MATCH("user_list", user_list_handler);
	HANDLE_IF_MATCH("overview", overview_handler);

	printf("Unknown message type %s, ignored.", command.c_str());
}

void server_message_handler(const std::vector<uint8_t>& packet) {
	ServerMessage message = ServerMessage::fromPacket(packet);
	if (message.type == "text") {
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(" > %s\n", message.content.c_str());
	}
	if (message.type == "file_hint") {
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(" uploaded file %s\n", message.content.c_str());
	}
	if (message.type == "join") {
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf("joined the group.");
	}
	if (message.type == "leave") {
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf("left the group.");
	}
	if (message.type == "history_hint") {
		setColor(Color::GRAY);
		printf("Above are history messages.");
		setColor(Color::WHITE);
		printf("left the group.");
	}
	if (message.type == "server") {
		setColor(Color::RED);
		printf("Server");
		setColor(Color::WHITE);
		printf(" > %s\n", message.content.c_str());
	}
}

void user_list_handler(const std::vector<uint8_t>& packet) {
	UserList list = UserList::fromPacket(packet);
	printf("There are %d users online:\n", list.usernames.size());

	for (string username : list.usernames) {
		setColor(genColor(username));
		printf("%s", username.c_str());
		setColor(Color::WHITE);
		printf(", ");
	}
	printf("\n");
}

void overview_handler(const std::vector<uint8_t>& packet) {
	ServerOverview overview = ServerOverview::fromPacket(packet);
	printf("There are %zd users online.\n", overview.online_count);

	printf("There are %zd groups opened in this server:\n", overview.groups.size());

	for (auto it : overview.groups) {
		printf("    Group > %s (%zd)\n", it.first.c_str(), it.second);
	}

	printf("\nThere are %zd users have messaged you:\n", overview.dms.size());
	for (auto it : overview.dms) {
		string name = it.first;
		setColor(genColor(name));
		printf("  %s ", name.c_str());
		setColor(Color::WHITE);
		printf("(%zd)\n", it.second);
	}

	printf("\n");
	printf("  /overview : refresh overview\n");
	printf("  /join_group <GroupName> : Join a existing group\n");
	printf("  /create_group <GroupName> : Create a group\n");
	printf("  /remove_group <GroupName> : Remove an empty group\n");

	printf("  /join_dm <Username> : Direct message to somebody\n");
	printf("  /list : List online users");

}