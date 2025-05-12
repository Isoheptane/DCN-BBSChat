#include "session.h"
#include "ServerCommands.h"

#include <ctime>

using std::string;

Session::Session(std::string addr, uint16_t port) {
	this->state = SessionState::STATE_WAIT;
	char id[128];

	this->epAddr = addr;
	this->epPort = port;

	time_t t = time(NULL);
	
	sprintf_s(id, sizeof(id), "%s:%d:%d", this->epAddr.c_str(), this->epPort, t);
	this->session_id = string(id);
}

bool Session::packet_pending() {
	std::lock_guard<std::recursive_mutex> guard(this->packet_buffer_mutex);
	return !packet_buffer.empty();
}
void Session::packet_push(std::vector<uint8_t> packet) {
	std::lock_guard<std::recursive_mutex> guard(this->packet_buffer_mutex);
	packet_buffer.push_back(packet);
}
std::vector<uint8_t> Session::packet_take() {
	std::lock_guard<std::recursive_mutex> guard(this->packet_buffer_mutex);
	std::vector<uint8_t> packet = packet_buffer.front();
	packet_buffer.pop_front();
	return packet;
}
