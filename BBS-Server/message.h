#pragma once

#include <string>

using std::string;

struct Message
{
	int timestamp;
	string type;
	string sender;
	string content;
};