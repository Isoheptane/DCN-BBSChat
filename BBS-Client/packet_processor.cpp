#include "packet_processor.h"

#include "file_pool.h"

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
void file_send_handler(const std::vector<uint8_t>&packet);
void disconnect_handler(const std::vector<uint8_t>&packet);

void packet_processor(const std::vector<uint8_t>& packet) {
	std::string command = getPacketCommand(packet);

	// printf("Received command %s\n", command.c_str());
	HANDLE_IF_MATCH("message", server_message_handler);
	HANDLE_IF_MATCH("user_list", user_list_handler);
	HANDLE_IF_MATCH("overview", overview_handler);
	HANDLE_IF_MATCH("file_send", file_send_handler);
	HANDLE_IF_MATCH("disconnect", disconnect_handler);

	printf("Unknown message type %s, ignored.", command.c_str());
}

void server_message_handler(const std::vector<uint8_t>& packet) {
	ServerMessage message = ServerMessage::fromPacket(packet);
	if (message.type == "text") {
		setColor(genColor(message.sender));
		printf(" %s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(": %s\n", message.content.c_str());
	}
	if (message.type == "file_hint") {
		setColor(Color::WHITE);
		printf(" * ");
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(" uploaded file %s\n", message.content.c_str());
	}
	if (message.type == "join") {
		setColor(Color::WHITE);
		printf(" * ");
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(" joined the group.\n");
	}
	if (message.type == "leave") {
		setColor(Color::WHITE);
		printf(" * ");
		setColor(genColor(message.sender));
		printf("%s", message.sender.c_str());
		setColor(Color::WHITE);
		printf(" left the group.\n");
	}
	if (message.type == "history_hint") {
		setColor(Color::GRAY);
		printf(" ^ History Messages ^\n");
		setColor(Color::WHITE);
	}
	if (message.type == "server") {
		setColor(Color::RED);
		printf(" Server");
		setColor(Color::WHITE);
		printf(" > %s\n", message.content.c_str());
	}
	if (message.type == "welcome") {
		setColor(Color::LIGHT_YELLOW);
		printf("\n%s\n", message.content.c_str());
		setColor(Color::WHITE);
	}
}

void user_list_handler(const std::vector<uint8_t>& packet) {
	UserList list = UserList::fromPacket(packet);
	printf("There are %zd users online / in this group:\n  ", list.usernames.size());

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

	setColor(Color::YELLOW);
	printf("Server Overview: \n");
	setColor(Color::WHITE);

	printf("  There are %zd users online.\n", overview.online_count);

	printf("  There are %zd groups opened in this server:\n", overview.groups.size());

	for (auto it : overview.groups) {
		printf("    - %s (%zd)\n", it.first.c_str(), it.second);
	}

	printf("\n  There are %zd users have messaged you:\n", overview.dms.size());
	for (auto it : overview.dms) {
		string name = it.first;
		setColor(Color::WHITE);
		printf("    - ");
		setColor(genColor(name));
		printf("%s", name.c_str());
		setColor(Color::WHITE);
		printf(" (%zd)\n", it.second);
	}

	printf("\n");

	setColor(Color::YELLOW);
	printf("  /overview");
	setColor(Color::WHITE);
	printf(" : Refresh overview\n");

	setColor(Color::YELLOW);
	printf("  /join_group");
	setColor(Color::WHITE);
	printf(" <GroupName> : Join a existing group\n");

	setColor(Color::YELLOW);
	printf("  /create_group");
	setColor(Color::WHITE);
	printf(" <GroupName> : Create a group\n");

	setColor(Color::YELLOW);
	printf("  /remove_group");
	setColor(Color::WHITE);
	printf(" <GroupName> : Remove an empty group group\n");

	setColor(Color::YELLOW);
	printf("  /join_dm");
	setColor(Color::WHITE);
	printf(" <Username> : Direct message to somebody\n");

	setColor(Color::YELLOW);
	printf("  /list");
	setColor(Color::WHITE);
	printf(" : List online users\n");

	setColor(Color::YELLOW);
	printf("  /help");
	setColor(Color::WHITE);
	printf(" : List all commands\n");


	printf("\n");

}

void file_send_handler(const std::vector<uint8_t>& packet) {
	ServerFileSend command = ServerFileSend::fromPacket(packet);

	const static string prefix = "./download/";
	if (CreateDirectory(prefix.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		string actual_name = prefix + command.filename;
		if (!file_pool.appendBytes(actual_name, command.block)) {
			printf("Failed to write to file.\n");
		}
	}
	else {
		printf("Failed to create and store file to downloads\n");
	}
}

void disconnect_handler(const std::vector<uint8_t>& packet) {
	printf("Server closed connection.\n");
	exit(0);
}