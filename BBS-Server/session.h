#pragma once

#include <string>
#include <vector>
#include <deque>
#include <mutex>

enum SessionState {
	WAIT,	// Waiting for authentication
	LOBBY,	// In lobby
	DM,		// In direct message page
	GROUP,	// In group
};

class Session
{
private:
	std::deque<std::vector<uint8_t>> packet_buffer;
	std::mutex packet_buffer_mutex;

public:	
	std::string session_id;
	std::string user;

	SessionState state;
	std::string associated_data;

	Session();
	
	bool packet_pending();
	void packet_push(std::vector<uint8_t> packet);
	std::vector<uint8_t> packet_take();
	
};

