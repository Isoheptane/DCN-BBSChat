#pragma once

#include "session.h"
#include "ServerCommands.h"
#include "ClientMessageCommands.h"

#include <string>
#include <mutex>
#include <map>
#include <deque>
#include <set>

class Group
{
public:

	const static size_t MAX_HISTORY_SIZE = 30;

	std::string name;
	std::map<std::string, std::shared_ptr<Session>> users;
	std::recursive_mutex users_mutex;

	std::deque<ServerMessage> history;
	std::recursive_mutex history_mutex;

	Group(std::string name);
	
	void add_user(std::shared_ptr<Session> session);
	void remove_user(std::shared_ptr<Session> session);
	bool empty();

	size_t count();
	void broadcast(ServerMessage message);

	bool broadcast_chat(std::string sender, ClientMessage message);

	std::vector<ServerMessage> get_history();
	std::set<std::string> get_usernames();
};

