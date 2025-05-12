#pragma once

#include "session.h"
#include "message.h"

#include <string>
#include <mutex>
#include <map>
#include <deque>

class Group
{
private:
	std::string name;
	std::map<std::string, std::shared_ptr<Session>> users;
	std::mutex users_mutex;
	std::deque<Message> history;
public:
	Group(std::string name);
	
	void add_user(std::shared_ptr<Session> session);
	void remove_user(std::shared_ptr<Session> session);
	bool has_user(std::string name);
	bool empty();
};

