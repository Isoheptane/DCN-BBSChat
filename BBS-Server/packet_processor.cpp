#include "packet_processor.h"

#include "Commands.h"
#include "ClientAuthCommands.h"
#include "server.h"

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

	// printf("Received command %s\n", command.c_str());

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
	LoginCommand command = LoginCommand::fromPacket(packet);
	if (!global_server.verify_user(command.username, command.password)) {
		printf(
			"[%s:%d] Failed to authenticate as %s.\n",
			session.get()->epAddr.c_str(), session.get()->epPort,
			command.username.c_str()
		);
		session.get()->packet_push(ServerMessage::serverMessage("Authentication failed.").toPacket());
		session.get()->state = SessionState::STATE_CLOSED;
		return;
	}
	printf(
		"[%s:%d] %s logged in.\n",
		session.get()->epAddr.c_str(), session.get()->epPort,
		command.username.c_str()
	);

	session.get()->user = command.username;
	session.get()->state = STATE_LOBBY;
	// Send a overview
	session.get()->packet_push(global_server.get_overview(command.username).toPacket());
}

void register_handler(HANDLER_ARGS) {
	// TODO: Add already registered check
	// TODO: Add to user list
	RegisterCommand command = RegisterCommand::fromPacket(packet);
	// Check already registered
	if (global_server.exist_user(command.username)) {
		printf(
			"[%s:%d] Failed to register %s because there is already a user with same name.\n",
			session.get()->epAddr.c_str(), session.get()->epPort,
			command.username.c_str()
		);
		session.get()->packet_push(ServerMessage::serverMessage("User already registered.").toPacket());
		session.get()->state = SessionState::STATE_CLOSED;
		return;
	}
	global_server.new_user(command.username, command.password);
	printf(
		"[%s:%d] New user %s registered.\n", 
		session.get()->epAddr.c_str(), session.get()->epPort,
		command.username.c_str()
	);

	session.get()->user = command.username;
	global_server.add_session(session);
	session.get()->state = STATE_LOBBY;
	// Send a overview
	session.get()->packet_push(global_server.get_overview(command.username).toPacket());
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