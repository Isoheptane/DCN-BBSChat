#include "pch.h"
#include "ClientAuthCommands.h"
#include "Buffer.hpp"
#include <stdexcept>

using std::string;
using std::vector;

// Serialize LoginCommand to protocol-compliant packet
std::vector<uint8_t> LoginCommand::toPacket() const {
    if (username.size() > 255) throw std::length_error("Username too long (max 255 bytes)");
    std::vector<uint8_t> packet;
    // Command: "login"
    const std::string command = "login";
    append_uint8(packet, static_cast<uint8_t>(command.size()));
    append_vector(packet, command);
    // Username
    append_uint8(packet, static_cast<uint8_t>(username.size()));
    append_vector(packet, username);
    // Password (plain, will be hashed in Client.cpp)
    append_vector(packet, this->password);
    return packet;
}

// Parse LoginCommand from protocol-compliant packet
LoginCommand LoginCommand::fromPacket(const std::vector<uint8_t>& packet) {
    size_t pos = 0;
    // Command Length
    uint8_t cmdLen = take_uint8(packet, pos++);
    // Command string
    std::string cmd = take_string(packet, pos, cmdLen);
    pos += cmdLen;
    if (cmd != "login") throw std::runtime_error("Not a login packet");
    // Username Length
    uint8_t unameLen = take_uint8(packet, pos++);
    std::string username = take_string(packet, pos, unameLen);
    pos += unameLen;
    // Password (rest of packet)
    std::vector<uint8_t> password = take_bytes(packet, pos, 32);
    pos += 32;
    return LoginCommand{username, password};
}

// Serialize RegisterCommand to protocol-compliant packet
std::vector<uint8_t> RegisterCommand::toPacket() const {
    if (username.size() > 255) throw std::length_error("Username too long (max 255 bytes)");
    std::vector<uint8_t> packet;
    // Command: "register"
    const std::string command = "register";
    append_uint8(packet, static_cast<uint8_t>(command.size()));
    append_vector(packet, command);
    // Username
    append_uint8(packet, static_cast<uint8_t>(username.size()));
    append_vector(packet, username);
    // Password (plain, will be hashed in Client.cpp)
    append_vector(packet, this->password);

    return packet;
}

// Parse RegisterCommand from protocol-compliant packet
RegisterCommand RegisterCommand::fromPacket(const std::vector<uint8_t>& packet) {
    size_t pos = 0;
    // Command Length
    uint8_t cmdLen = take_uint8(packet, pos++);
    // Command string
    std::string cmd = take_string(packet, pos, cmdLen);
    pos += cmdLen;
    if (cmd != "register") throw std::runtime_error("Not a register packet");
    // Username Length
    uint8_t unameLen = take_uint8(packet, pos++);
    std::string username = take_string(packet, pos, unameLen);
    pos += unameLen;
    // Password (rest of packet)
    std::vector<uint8_t> password = take_bytes(packet, pos, 32);
    pos += 32;
    return RegisterCommand{ username, password };
}
