#include "packet_processor.h"

#include "Commands.h"

#include <string>

#define HANDLER_ARGS std::shared_ptr<Session> session, const std::vector<uint8_t>& packet

#define HANDLE_IF_MATCH(x, handler) if (command == (x)) { \
	handler(session, packet); \
	return; \
}

void login_handler(HANDLER_ARGS);
void register_handler(HANDLER_ARGS);
void lobby_handler(HANDLER_ARGS);
void overview_handler(HANDLER_ARGS);
void create_group_handler(HANDLER_ARGS);
void join_group_handler(HANDLER_ARGS);
void remove_group_handler(HANDLER_ARGS);
void join_dm_handler(HANDLER_ARGS);
void list_handler(HANDLER_ARGS);
void message_handler(HANDLER_ARGS);
void file_download_handler(HANDLER_ARGS);
void file_delete_handler(HANDLER_ARGS);
void file_upload_handler(HANDLER_ARGS);
void logout_handler(HANDLER_ARGS);
void heartbeat_handler(HANDLER_ARGS);

void packet_processor(std::shared_ptr<Session> session, const std::vector<uint8_t>& packet) {
	std::string command = getPacketCommand(packet);

	HANDLE_IF_MATCH("login", login_handler);
	HANDLE_IF_MATCH("register", register_handler);
	HANDLE_IF_MATCH("lobby", lobby_handler);
	HANDLE_IF_MATCH("overview", overview_handler);
	HANDLE_IF_MATCH("create_group", create_group_handler);
	HANDLE_IF_MATCH("join_group", join_group_handler);
	HANDLE_IF_MATCH("remove_group", remove_group_handler);
	HANDLE_IF_MATCH("join_dm", join_dm_handler);
	HANDLE_IF_MATCH("list", list_handler);
	HANDLE_IF_MATCH("message", message_handler);
	HANDLE_IF_MATCH("file_download", file_download_handler);
	HANDLE_IF_MATCH("file_delete", file_delete_handler);
	HANDLE_IF_MATCH("file_upload", file_upload_handler);
	HANDLE_IF_MATCH("logout", logout_handler);
	HANDLE_IF_MATCH("heartbeat", logout_handler);

	printf("Unknown message type %s, ignored.", command.c_str());
}

void login_handler(HANDLER_ARGS) {

}

void register_handler(HANDLER_ARGS) {

}

void lobby_handler(HANDLER_ARGS) {

}

void overview_handler(HANDLER_ARGS) {

}

void create_group_handler(HANDLER_ARGS) {

}

void join_group_handler(HANDLER_ARGS) {

}

void remove_group_handler(HANDLER_ARGS) {

}

void join_dm_handler(HANDLER_ARGS) {

}

void list_handler(HANDLER_ARGS) {

}

void message_handler(HANDLER_ARGS) {

}

void file_download_handler(HANDLER_ARGS) {

}

void file_delete_handler(HANDLER_ARGS) {

}

void file_upload_handler(HANDLER_ARGS) {

}

void logout_handler(HANDLER_ARGS) {

}

void heartbeat_handler(HANDLER_ARGS) {

}