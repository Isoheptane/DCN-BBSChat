#pragma once

#include "user.h"
#include "group.h"

#include <map>

class Server
{
public:
	std::map<std::string, User> users;
	std::map<std::string, Group> groups;
};

extern Server server;

