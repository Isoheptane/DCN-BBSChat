#pragma once
#include <vector>
#include <string>
#include <cstdint>

// Generate login/register command packets according to protocol
// Returns a byte vector ready to send to server
std::vector<uint8_t> make_login_packet(const std::string& username, const std::string& password);

// Returns a byte vector for register command
std::vector<uint8_t> make_register_packet(const std::string& username, const std::string& password);

