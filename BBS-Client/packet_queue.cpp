#include "packet_queue.h"

PacketQueue::PacketQueue() {

}

bool PacketQueue::available() {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	return !this->queue.empty();
}

void PacketQueue::push(std::vector<uint8_t> packet) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	queue.push_back(packet);
}

std::vector<uint8_t> PacketQueue::take() {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	std::vector<uint8_t> packet = this->queue.front();
	this->queue.pop_front();
	return packet;
}