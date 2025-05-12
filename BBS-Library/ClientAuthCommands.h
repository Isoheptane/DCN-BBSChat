#pragma once
#include <vector>
#include <string>
#include <cstdint>

// Generate login/register command packets according to protocol
// Returns a byte vector ready to send to server

// Structure for login command according to protocol
struct LoginCommand {
    std::string username; // plain username
    std::string password; // plain password (will be hashed)
    // Serialize to protocol-compliant packet
    std::vector<uint8_t> toPacket() const;
    // Parse from protocol-compliant packet
    static LoginCommand fromPacket(const std::vector<uint8_t>& packet);
};

// Structure for register command according to protocol
struct RegisterCommand {
    std::string username; // plain username
    std::string password; // plain password (will be hashed)
    // Serialize to protocol-compliant packet
    std::vector<uint8_t> toPacket() const;
    // Parse from protocol-compliant packet
    static RegisterCommand fromPacket(const std::vector<uint8_t>& packet);
};



std::vector<uint8_t> make_login_packet(const std::string& username, const std::string& password);

// Returns a byte vector for register command
std::vector<uint8_t> make_register_packet(const std::string& username, const std::string& password);

