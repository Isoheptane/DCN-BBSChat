#include "session.h"
#include "Commands.h"


Session::Session() {
	this->state = SessionState::WAIT;
}

bool Session::packet_pending() {
	std::lock_guard<std::mutex> guard(this->packet_buffer_mutex);
	return !packet_buffer.empty();
}
void Session::packet_push(std::vector<uint8_t> packet) {
	std::lock_guard<std::mutex> guard(this->packet_buffer_mutex);
	packet_buffer.push_back(packet);
}
std::vector<uint8_t> Session::packet_take() {
	std::lock_guard<std::mutex> guard(this->packet_buffer_mutex);
	std::vector<uint8_t> packet = packet_buffer.front();
	packet_buffer.pop_front();
	return packet;
}