#include "pch.h"
#include "ClientAuthCommands.h"
#include "Crypto.h"
#include <stdexcept>

using Crypto::SHA256::SHA256;
using std::string;
using std::vector;

// Serialize LoginCommand to protocol-compliant packet
vector<uint8_t> LoginCommand::toPacket() const {
    if (username.size() > 255) throw std::length_error("Username too long (max 255 bytes)");
    vector<uint8_t> packet;
    // Command: "login"
    const string command = "login";
    packet.push_back(static_cast<uint8_t>(command.size()));
    packet.insert(packet.end(), command.begin(), command.end());
    // Username
    packet.push_back(static_cast<uint8_t>(username.size()));
    packet.insert(packet.end(), username.begin(), username.end());
    // Password (SHA-256 hash, 32 bytes)
    SHA256 sha;
    sha.update(password);
    auto digest = sha.digest();
    packet.insert(packet.end(), digest.begin(), digest.end());
    return packet;
}

// Serialize RegisterCommand to protocol-compliant packet
vector<uint8_t> RegisterCommand::toPacket() const {
    if (username.size() > 255) throw std::length_error("Username too long (max 255 bytes)");
    vector<uint8_t> packet;
    // Command: "register"
    const string command = "register";
    packet.push_back(static_cast<uint8_t>(command.size()));
    packet.insert(packet.end(), command.begin(), command.end());
    // Username
    packet.push_back(static_cast<uint8_t>(username.size()));
    packet.insert(packet.end(), username.begin(), username.end());
    // Password (SHA-256 hash, 32 bytes)
    SHA256 sha;
    sha.update(password);
    auto digest = sha.digest();
    packet.insert(packet.end(), digest.begin(), digest.end());
    return packet;
}
