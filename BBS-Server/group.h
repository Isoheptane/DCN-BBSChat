#pragma once

#include "session.h"
#include "ServerCommands.h"
#include "ClientMessageCommands.h"

#include <string>
#include <mutex>
#include <map>
#include <deque>

class Group
{
public:

	std::string name;
	std::map<std::string, std::shared_ptr<Session>> users;
	std::recursive_mutex users_mutex;
	std::deque<ServerMessage> history;

	Group(std::string name);
	
	void add_user(std::shared_ptr<Session> session);
	void remove_user(std::shared_ptr<Session> session);
	bool has_user(std::string name);
	bool empty();

	size_t count();
	bool broadcast(std::string sender, ClientMessage message);
};

