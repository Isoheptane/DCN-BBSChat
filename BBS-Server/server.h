#pragma once

#include "user.h"
#include "group.h"
#include "session.h"
#include "ServerCommands.h"
#include "FileCache.h"

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

	WritingFilePool file_pool;

	// Users
	void new_user(std::string username, std::vector<uint8_t> password);
	bool exist_user(std::string username);
	bool verify_user(std::string username, std::vector<uint8_t> password);

	/// User after exist check
	std::shared_ptr<User> get_user(std::string username);

	// Groups
	void new_group(std::string name);
	bool exist_group(std::string name);
	bool remove_group(std::string name);
	
	/// User after exist check
	std::shared_ptr<Group> get_group(std::string name);

	void remove_from_group(std::shared_ptr<Session> session);

	// Sessions
	void add_session(std::shared_ptr<Session> session);
	void remove_session(std::shared_ptr<Session> session);

	ServerOverview get_overview(std::string user);
	UserList get_userlist();

	// File operations
	bool append_to_file(std::string name, const std::vector<uint8_t> bytes);

};

extern Server global_server;

