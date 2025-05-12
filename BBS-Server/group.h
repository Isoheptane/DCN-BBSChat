#pragma once

#include <string>
#include <mutex>
#include <set>
#include <deque>

class Group
{
private:
	std::string name;
	std::set<std::string> users;
	std::mutex users_mutex;
	
public:
	Group(std::string name);
	
	void add_user(std::string name);
	void remove_user(std::string name);
	bool has_user(std::string name);
	bool empty();
	
};

