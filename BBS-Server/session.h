#pragma once

#include <string>
#include <vector>
#include <deque>
#include <mutex>

enum SessionState {
	STATE_WAIT,		// Waiting for authentication
	STATE_LOBBY,	// In lobby
	STATE_DM,		// In direct message page
	STATE_GROUP,	// In group
	STATE_CLOSED	// Closed by server
};

class Session
{
private:
	std::deque<std::vector<uint8_t>> packet_buffer;
	std::recursive_mutex packet_buffer_mutex;

public:	
	std::string epAddr;
	uint16_t epPort;

	std::string session_id;
	std::string user;

	SessionState state;
	std::string associated_data;

	Session(std::string addr, uint16_t port);
	
	bool packet_pending();
	void packet_push(std::vector<uint8_t> packet);
	std::vector<uint8_t> packet_take();

};

