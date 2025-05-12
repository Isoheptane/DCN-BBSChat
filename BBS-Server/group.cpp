#include "group.h"

Group::Group(std::string name) {

}

void Group::add_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::mutex> guard(this->users_mutex);
	this->users[session.get()->user] = session;
}

void Group::remove_user(std::shared_ptr<Session> session) {
	std::lock_guard<std::mutex> guard(this->users_mutex);
	this->users.erase(session.get()->user);
}

bool Group::has_user(std::string name) {
	std::lock_guard<std::mutex> guard(this->users_mutex);
	return this->users.find(name) != this->users.end();
}

bool Group::empty() {
	std::lock_guard<std::mutex> guard(this->users_mutex);
	return this->users.empty();
}