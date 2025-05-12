#pragma once

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include <mutex>
#include <tuple>


#include "ServerCommands.h"

using std::string;

class User
{
private:
	// Message to be sent
	std::map<string, std::deque<ServerMessage>> dm_pending;
	// DM Pending mutex
	std::recursive_mutex dm_pending_mutex;
public:
	string name;
	std::vector<uint8_t> password;

	User();
	User(string name, std::vector<uint8_t> password);

	void push_direct_message(string sender, ServerMessage message);

	std::vector<ServerMessage> fetch_direct_message(string sender);

	std::map<string, size_t> get_direct_message_counts();
};

