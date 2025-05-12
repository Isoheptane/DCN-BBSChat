#pragma once
#include <vector>
#include <string>
#include <cstdint>

// Generate login/register command packets according to protocol
// Returns a byte vector ready to send to server

// Structure for login command according to protocol
struct LoginCommand {
    std::string username; // plain username
    std::vector<uint8_t> password; // plain password hashed (SHA-256)
    // Serialize to protocol-compliant packet
    std::vector<uint8_t> toPacket() const;
    // Parse from protocol-compliant packet
    static LoginCommand fromPacket(const std::vector<uint8_t>& packet);
};

// Structure for register command according to protocol
struct RegisterCommand {
    std::string username; // plain username
    std::vector<uint8_t> password; // plain password hashed (SHA-256)
    // Serialize to protocol-compliant packet
    std::vector<uint8_t> toPacket() const;
    // Parse from protocol-compliant packet
    static RegisterCommand fromPacket(const std::vector<uint8_t>& packet);
};

