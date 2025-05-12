#include "group.h"

Group::Group(std::string name) : name(name) {

}

void Group::add_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	this->users[session.get()->session_id] = session;
}

void Group::remove_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	this->users.erase(session.get()->session_id);
}

bool Group::empty() {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.empty();
}

size_t Group::count() {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.size();
}

void Group::broadcast(ServerMessage message) {
	std::vector<uint8_t> packet = message.toPacket();

	// Add to history
	{
		std::lock_guard<std::recursive_mutex> guard(this->history_mutex);
		this->history.push_back(message);
		while (history.size() > MAX_HISTORY_SIZE) {
			history.pop_front();
		}
	}
	// Broadcast to sessions
	{
		std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
		for (auto session : this->users) {
			session.second.get()->packet_push(packet);
		}
	}
}

bool Group::broadcast_chat(std::string sender, ClientMessage message) {
	if (message.type != "text" && message.type != "file_hint") {
		return false;
	}
	ServerMessage sm = ServerMessage(message.type, sender, message.content);

	this->broadcast(sm);
}

std::vector<ServerMessage> Group::get_history() {
	std::vector<ServerMessage> messages;

	std::lock_guard<std::recursive_mutex> guard(this->history_mutex);
	messages.insert(messages.end(), this->history.begin(), this->history.end());

	return messages;
}

std::set<std::string> Group::get_usernames() {
	std::set<std::string> names;
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	for (auto session : this->users) {
		names.insert(session.second.get()->user);
	}
	return names;
}