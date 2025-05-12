#pragma once
#include <vector>
#include <deque>
#include <mutex>

class PacketQueue
{
private: 
	std::deque<std::vector<uint8_t>> queue;
	std::recursive_mutex mutex;

public:
	PacketQueue();

	bool available();
	void push(std::vector<uint8_t> packet);
	std::vector<uint8_t> take();
};

