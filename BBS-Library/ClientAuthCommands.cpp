#include "pch.h"
#include "ClientAuthCommands.h"
#include "Buffer.hpp"
#include "Crypto.h"

// Use Crypto::SHA256 for password hashing
using Crypto::SHA256::SHA256;
using std::string;
using std::vector;

// Generate a login command packet according to protocol
std::vector<uint8_t> make_login_packet(const std::string& username, const std::string& password) {
    std::vector<uint8_t> packet;
    // 1. Command: "login"
    const string command = "login";
    packet.push_back(static_cast<uint8_t>(command.size())); // Command Length
    packet.insert(packet.end(), command.begin(), command.end()); // Command
    // 2. Username
    packet.push_back(static_cast<uint8_t>(username.size())); // Username Length
    packet.insert(packet.end(), username.begin(), username.end()); // Username
    // 3. Password (SHA-256 hash, 32 bytes)
    SHA256 sha;
    sha.update(password);
    auto digest = sha.digest();
    packet.insert(packet.end(), digest.begin(), digest.end());
    return packet;
}

// Generate a register command packet according to protocol
std::vector<uint8_t> make_register_packet(const std::string& username, const std::string& password) {
    std::vector<uint8_t> packet;
    // 1. Command: "register"
    const string command = "register";
    packet.push_back(static_cast<uint8_t>(command.size())); // Command Length
    packet.insert(packet.end(), command.begin(), command.end()); // Command
    // 2. Username
    packet.push_back(static_cast<uint8_t>(username.size())); // Username Length
    packet.insert(packet.end(), username.begin(), username.end()); // Username
    // 3. Password (SHA-256 hash, 32 bytes)
    SHA256 sha;
    sha.update(password);
    auto digest = sha.digest();
    packet.insert(packet.end(), digest.begin(), digest.end());
    return packet;
}
