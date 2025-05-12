#pragma once

#include "user.h"
#include "group.h"
#include "session.h"
#include "ServerCommands.h"

#include <map>
#include <mutex>
#include <string>

class Server
{
public:
	std::map<std::string, std::shared_ptr<User>> users;
	std::recursive_mutex users_mutex;

	std::map<std::string, std::shared_ptr<Group>> groups;
	std::recursive_mutex groups_mutex;

	std::map<std::string, std::shared_ptr<Session>> sessions;
	std::recursive_mutex sessions_mutex;

	void new_user(std::string username, std::vector<uint8_t> password);
	bool exist_user(std::string username);
	bool verify_user(std::string username, std::vector<uint8_t> password);

	/// Use after exist_user.
	std::shared_ptr<User> get_user(std::string username);

	void new_group(std::string name);
	bool exist_group(std::string name);
	bool remove_group(std::string name);
	std::shared_ptr<Group> get_group(std::string name);

	void add_session(std::shared_ptr<Session> session);
	void remove_session(std::string session_id);

	ServerOverview get_overview(std::string user);
};

extern Server global_server;

