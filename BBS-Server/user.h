#pragma once

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include <mutex>
#include <tuple>

#include "message.h"

using std::string;

class User
{
public:
	string name;
	string password;
	// Message to be sent
	std::map<string, std::deque<Message>> dm_pending;
	// Pending mutex
	std::mutex dm_pending_mutex;

	User(string name, string password);

	void push_direct_message(string sender, Message message);

	std::vector<Message> fetch_direct_message(string sender);

	std::map<string, size_t> get_direct_message_counts();
};

