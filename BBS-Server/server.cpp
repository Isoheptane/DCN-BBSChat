#include "server.h"

using std::string;
using std::vector;

Server global_server;

void Server::new_user(string username, vector<uint8_t> password) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	auto user = std::make_shared<User>(username, password);
	this->users[username] = user;
}
bool Server::exist_user(string username) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users.find(username) != this->users.end();
}
bool Server::verify_user(string username, vector<uint8_t> password) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	if (!this->exist_user(username)) {
		return false;
	}
	auto u = this->users[username];
	return u.get()->password == password;
}

std::shared_ptr<User> Server::get_user(std::string username) {
	std::lock_guard<std::recursive_mutex> guard(this->users_mutex);
	return this->users[username];
}


void Server::new_group(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->groups_mutex);
	auto group = std::make_shared<Group>(name);
}

bool Server::exist_group(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->groups_mutex);
	return this->groups.find(name) != this->groups.end();
}

bool Server::remove_group(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->groups_mutex);
	if (!this->exist_group(name)) {
		return false;
	};
	auto g = this->groups[name];
	if (!g.get()->empty()) {
		return false;
	}
	this->groups.erase(name);
}

std::shared_ptr<Group> Server::get_group(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->groups_mutex);
	return this->groups[name];
}

ServerOverview Server::get_overview(string user) {
	ServerOverview overview;
	{
		std::lock_guard<std::recursive_mutex> guard(this->sessions_mutex);
		std::set<string> usernames;
		for (auto it : this->sessions) {
			usernames.insert(it.second.get()->user);
		}
		overview.online_count = usernames.size();
	}
	{
		std::lock_guard<std::recursive_mutex> guard(this->groups_mutex);
		for (auto it : this->groups) {
			string name = it.second.get()->name;
			size_t count = it.second.get()->count();
			overview.groups[name] = count;
		}
	}
	if (this->exist_user(user)) {
		auto u = this->get_user(user);
		overview.dms = u.get()->get_direct_message_counts();
	}

	return overview;
}

void Server::add_session(std::shared_ptr<Session> session) {
	std::lock_guard<std::recursive_mutex> guard(this->sessions_mutex);
	this->sessions[session.get()->session_id] = session;
}

void Server::remove_session(std::string session_id) {
	std::lock_guard<std::recursive_mutex> guard(this->sessions_mutex);
	this->sessions.erase(session_id);
}

