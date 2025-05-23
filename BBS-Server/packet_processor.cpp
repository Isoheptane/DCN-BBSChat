#include "packet_processor.h"

#include "Commands.h"
#include "ClientAuthCommands.h"
#include "ClientGroupDMCommands.h"
#include "ClientMessageCommands.h"
#include "ClientFileCommands.h"
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

	// Below handlers requires the client to be authenticated.
	// This piece of code is a guard code.
	// This should not be displayed because any user should be authenticated first.
	if (session.get()->state == STATE_WAIT || session.get()->state == STATE_CLOSED) {
		session.get()->packet_push(ServerMessage::serverMessage("You have not authenticated.").toPacket());
		return;
	}

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

	printf("[%s:%d] Unknown message type %s, ignored.\n", session.get()->epAddr.c_str(), session.get()->epPort, command.c_str());
}

const static string motd = R"foo(
$$$$$$$\   $$$$$$\  $$\   $$\          $$$$$$\  $$\   $$\  $$$$$$\ $$$$$$$$\ 
$$  __$$\ $$  __$$\ $$$\  $$ |        $$  __$$\ $$ |  $$ |$$  __$$\\__$$  __|
$$ |  $$ |$$ /  \__|$$$$\ $$ |        $$ /  \__|$$ |  $$ |$$ /  $$ |  $$ |   
$$ |  $$ |$$ |      $$ $$\$$ |$$$$$$\ $$ |      $$$$$$$$ |$$$$$$$$ |  $$ |   
$$ |  $$ |$$ |      $$ \$$$$ |\______|$$ |      $$  __$$ |$$  __$$ |  $$ |   
$$ |  $$ |$$ |  $$\ $$ |\$$$ |        $$ |  $$\ $$ |  $$ |$$ |  $$ |  $$ |   
$$$$$$$  |\$$$$$$  |$$ | \$$ |        \$$$$$$  |$$ |  $$ |$$ |  $$ |  $$ |   
\_______/  \______/ \__|  \__|         \______/ \__|  \__|\__|  \__|  \__|   
)foo";


void login_handler(HANDLER_ARGS) {
	LoginCommand command = LoginCommand::fromPacket(packet);
	if (!global_server.verify_user(command.username, command.password)) {
		printf(
			"[%s:%d] Failed to authenticate as %s.\n",
			session.get()->epAddr.c_str(), session.get()->epPort,
			command.username.c_str()
		);
		session.get()->packet_push(ServerMessage::serverMessage("Authentication failed.").toPacket());
		session.get()->packet_push(makeBodylessPacket("disconnect"));
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
	session.get()->packet_push(ServerMessage::welcomeMessage(motd).toPacket());
	session.get()->packet_push(ServerMessage::serverMessage("Welcome back!").toPacket());
	session.get()->packet_push(global_server.get_overview(session.get()->user).toPacket());
}

void register_handler(HANDLER_ARGS) {
	RegisterCommand command = RegisterCommand::fromPacket(packet);
	// Check already registered
	if (global_server.exist_user(command.username)) {
		printf(
			"[%s:%d] Failed to register %s because there is already a user with same name.\n",
			session.get()->epAddr.c_str(), session.get()->epPort,
			command.username.c_str()
		);
		session.get()->packet_push(ServerMessage::serverMessage("User already registered.").toPacket());
		session.get()->packet_push(makeBodylessPacket("disconnect"));
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
	session.get()->packet_push(ServerMessage::welcomeMessage(motd).toPacket());
	session.get()->packet_push(ServerMessage::serverMessage("You have successully registered.").toPacket());
	session.get()->packet_push(global_server.get_overview(session.get()->user).toPacket());
}

void lobby_handler(HANDLER_ARGS) {

	global_server.remove_from_group(session);

	session.get()->state = STATE_LOBBY;

	session.get()->packet_push(ServerMessage::serverMessage("You have come back to the lobby.").toPacket());
	session.get()->packet_push(global_server.get_overview(session.get()->user).toPacket());
}

void overview_handler(HANDLER_ARGS) {
	session.get()->packet_push(global_server.get_overview(session.get()->user).toPacket());
}

void create_group_handler(HANDLER_ARGS) {

	// User can't create a group if the user is already in a group
	if (session.get()->state == SessionState::STATE_GROUP) {
		session.get()->packet_push(ServerMessage::serverMessage("You are already in a group. Use /lobby command to go back to lobby before joining a new group.").toPacket());
		return;
	}

	ClientGroupDMCommands command = ClientGroupDMCommands::fromPacket(packet);
	
	if (global_server.exist_group(command.name)) {
		session.get()->packet_push(ServerMessage::serverMessage("There already exist a group with the same name!").toPacket());
		return;
	}
	
	// Create new group and send our dear user to the group
	global_server.new_group(command.name);
	auto g = global_server.get_group(command.name);
	g.get()->add_user(session);

	session.get()->state = SessionState::STATE_GROUP;
	session.get()->associated_data = command.name;
	session.get()->packet_push(ServerMessage::serverMessage("You have created and joined a group.").toPacket());
	g.get()->broadcast(ServerMessage::joinMessage(session.get()->user));
}

void join_group_handler(HANDLER_ARGS) {

	// User can't join a group if the user is already in a group
	if (session.get()->state == SessionState::STATE_GROUP) {
		session.get()->packet_push(ServerMessage::serverMessage("You are already in a group. Use /lobby command to go back to lobby before joining a new group.").toPacket());
		return;
	}
	
	// Receive content
	ClientGroupDMCommands command = ClientGroupDMCommands::fromPacket(packet);

	if (!global_server.exist_group(command.name)) {
		session.get()->packet_push(ServerMessage::serverMessage("Group not exist.").toPacket());
		return;
	}

	// Join the group
	auto g = global_server.get_group(command.name);

	std::vector<ServerMessage> history = g.get()->get_history();
	history.push_back(ServerMessage::historyHintMessage());
	g.get()->add_user(session);

	// Set the state of the user
	session.get()->state = SessionState::STATE_GROUP;
	session.get()->associated_data = command.name;
	// Send history and broadcast
	for (ServerMessage msg : history) {
		session.get()->packet_push(msg.toPacket());
	}
	g.get()->broadcast(ServerMessage::joinMessage(session.get()->user));
}

void remove_group_handler(HANDLER_ARGS) {
	ClientGroupDMCommands command = ClientGroupDMCommands::fromPacket(packet);

	if (!global_server.exist_group(command.name)) {
		session.get()->packet_push(ServerMessage::serverMessage("Group not exist.").toPacket());
		return;
	}

	bool result = global_server.remove_group(command.name);

	if (result) {
		session.get()->packet_push(ServerMessage::serverMessage("Group deleted.").toPacket());
	}
	else {
		session.get()->packet_push(ServerMessage::serverMessage("Failed to delete the group. Maybe the group is not empty.").toPacket());
	}
}

void join_dm_handler(HANDLER_ARGS) {
	// User can't join DM if the user is already in a group
	if (session.get()->state == SessionState::STATE_GROUP) {
		session.get()->packet_push(ServerMessage::serverMessage("You in a group right now. Use /lobby command to go back to lobby before joining a new group.").toPacket());
		return;
	}

	ClientGroupDMCommands command = ClientGroupDMCommands::fromPacket(packet);

	if (!global_server.exist_user(command.name)) {
		session.get()->packet_push(ServerMessage::serverMessage("User does not exist.").toPacket());
		return;
	}

	// Sent the uset to DM state
	session.get()->packet_push(ServerMessage::serverMessage("You have joined direct message with the user.").toPacket());
	session.get()->state = SessionState::STATE_DM;
	session.get()->associated_data = command.name;
}

void list_handler(HANDLER_ARGS) {
	if (session.get()->state == SessionState::STATE_GROUP) {
		if (!global_server.exist_group(session.get()->associated_data)) {
			session.get()->packet_push(ServerMessage::serverMessage("You doesn't seem to be in a existing group. Try /lobby to get back to lobby.").toPacket());
			return;
		}
		auto g = global_server.get_group(session.get()->associated_data);
		auto names = g.get()->get_usernames();

		UserList list;
		list.usernames = names;
		session.get()->packet_push(list.toPacket());
		return;
	}
	
	UserList list = global_server.get_userlist();
	session.get()->packet_push(list.toPacket());
}

void message_handler(HANDLER_ARGS) {
	ClientMessage message = ClientMessage::fromPacket(packet);
	if (session.get()->state == SessionState::STATE_LOBBY) {
		session.get()->packet_push(ServerMessage::serverMessage("You are in the lobby. Join a group or direct message to send messages.").toPacket());
		return;
	}
	if (session.get()->state == SessionState::STATE_GROUP) {
		if (!global_server.exist_group(session.get()->associated_data)) {
			session.get()->packet_push(ServerMessage::serverMessage("You doesn't seem to be in a existing group. Try /lobby to get back to lobby.").toPacket());
			return;
		}
		auto g = global_server.get_group(session.get()->associated_data);

		g.get()->broadcast_chat(session.get()->user, message);
	}
	if (session.get()->state == SessionState::STATE_DM) {
		if (!global_server.exist_user(session.get()->associated_data)) {
			session.get()->packet_push(ServerMessage::serverMessage("You doesn't seem to be in a valid DM. Try /lobby to get back to lobby.").toPacket());
			return;
		}
		auto u = global_server.get_user(session.get()->associated_data);
		
		ServerMessage sm = ServerMessage(message.type, session.get()->user, message.content);
		u.get()->push_direct_message(session.get()->user, sm);
		// Sendback to sender for display
		session.get()->packet_push(sm.toPacket());
	}
}

void file_download_handler(HANDLER_ARGS) {
	ClientFileCommand command = ClientFileCommand::fromPacket(packet);

	string path = "./upload/" + command.fileName;
	std::ifstream readfs = std::ifstream(path, std::ifstream::binary);
	if (readfs.fail()) {
		session.get()->packet_push(ServerMessage::serverMessage("File does not exist or failed to open file.").toPacket());
		return;
	}

	size_t prev_sent_size = 0;
	size_t sent_size = 0;

	char buffer[32768];
	while (true) {
		// printf(".");
		readfs.read(buffer, sizeof(buffer));
		size_t block_size = readfs.gcount();

		sent_size += block_size;
		if (sent_size >= prev_sent_size + (1024 * 1024)) {
			prev_sent_size = sent_size;
			
			char format_str[256];
			sprintf_s(format_str, sizeof(format_str), "%zd MiB Transmitted", prev_sent_size / (1024 * 1024));
			string hint = string(format_str);
			session.get()->packet_push(ServerMessage::serverMessage(hint).toPacket());
		}

		ServerFileSend resp;
		resp.filename = command.fileName;

		for (size_t i = 0; i < block_size; i++)
			resp.block.push_back(buffer[i]);

		session.get()->packet_push(resp.toPacket());

		if (block_size < sizeof(buffer)) {
			break;
		}
	}

	session.get()->packet_push(ServerMessage::serverMessage("File transmission complete.").toPacket());
}

void file_delete_handler(HANDLER_ARGS) {
	ClientFileCommand command = ClientFileCommand::fromPacket(packet);

	string path = "./upload/" + command.fileName;
	std::ifstream fs = std::ifstream(path);
	if (!fs.is_open()) {
		session.get()->packet_push(ServerMessage::serverMessage("File does not exist.").toPacket());
		return;
	}
	fs.close();
	if (remove(path.c_str()) == 0) {
		session.get()->packet_push(ServerMessage::serverMessage("File deleted.").toPacket());
		return;
	}
	else {
		session.get()->packet_push(ServerMessage::serverMessage("Failed to delete file.").toPacket());
		return;
	}
}

void file_upload_handler(HANDLER_ARGS) {
	ClientFileCommand command = ClientFileCommand::fromPacket(packet);
	if (!global_server.append_to_file(command.fileName, command.fileData)) {
		session.get()->packet_push(ServerMessage::serverMessage("File append operation failed.").toPacket());
		return;
	}
}

void logout_handler(HANDLER_ARGS) {
	
}

void heartbeat_handler(HANDLER_ARGS) {

}

void check_dm(std::shared_ptr<Session> session) {
	if (session->state != STATE_DM) {
		return;
	}
	if (!global_server.exist_user(session.get()->user)) {
		return;
	}
	auto u = global_server.get_user(session.get()->user);
	auto messages = u.get()->fetch_direct_message(session.get()->associated_data);
	for (ServerMessage message : messages) {
		session.get()->packet_push(message.toPacket());
	}
}