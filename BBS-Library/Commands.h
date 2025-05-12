#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

std::string getPacketCommand(const std::vector<uint8_t>& packet);

std::vector<uint8_t> makeBodylessPacket(std::string command);