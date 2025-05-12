#include "group.h"

Group::Group(std::string name) {

}

void Group::add_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	this->users[session.get()->user] = session;
}

void Group::remove_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	this->users.erase(session.get()->user);
}

bool Group::has_user(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.find(name) != this->users.end();
}

bool Group::empty() {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.empty();
}

size_t Group::count() {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.size();
}

bool Group::broadcast(std::string sender, ClientMessage message) {
	if (message.type != "text" && message.type != "file_hint") {
		return false;
	}
	ServerMessage sm = ServerMessage(message.type, sender, message.content);
	std::vector<uint8_t> packet = sm.toPacket();

	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	for (auto session : this->users) {
		session.second.get()->packet_push(packet);
	}
	return true;
}