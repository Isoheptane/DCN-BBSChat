#include "pch.h"
#include "ClientGroupDMCommands.h"
#include "Buffer.hpp"

using std::string;
using std::vector;

// Constructor for group or DM command
ClientGroupDMCommands::ClientGroupDMCommands(std::string command, std::string groupName, std::string username)
    : command(std::move(command)), groupName(std::move(groupName)), username(std::move(username)) {
}

// Static method to create a "create group" command
ClientGroupDMCommands ClientGroupDMCommands::createGroupCommand(const std::string& groupName) {
    return ClientGroupDMCommands("create_group", groupName);
}

// Static method to create a "join group" command
ClientGroupDMCommands ClientGroupDMCommands::joinGroupCommand(const std::string& groupName) {
    return ClientGroupDMCommands("join_group", groupName);
}

// Static method to create a "remove group" command
ClientGroupDMCommands ClientGroupDMCommands::removeGroupCommand(const std::string& groupName) {
    return ClientGroupDMCommands("remove_group", groupName);
}

// Static method to create a "join DM" command
ClientGroupDMCommands ClientGroupDMCommands::joinDMCommand(const std::string& username) {
    return ClientGroupDMCommands("join_dm", "", username);
}

// Serialize the command into a protocol-compliant packet
vector<uint8_t> ClientGroupDMCommands::toPacket() const {
    vector<uint8_t> buffer;

    // Append command length and command string
    append_uint8(buffer, static_cast<uint8_t>(command.size()));
    append_string(buffer, command);

    // For "create_group", "join_group", and "remove_group", include the group name
    if (command == "create_group" || command == "join_group" || command == "remove_group") {
        append_uint8(buffer, static_cast<uint8_t>(groupName.size()));
        append_string(buffer, groupName);
    }

    // For "join_dm", include the username
    if (command == "join_dm") {
        append_uint8(buffer, static_cast<uint8_t>(username.size()));
        append_string(buffer, username);
    }

    return buffer;
}

// Helper function to append a uint8 value to the buffer
void ClientGroupDMCommands::append_uint8(std::vector<uint8_t>& buffer, uint8_t value) {
    buffer.push_back(value);
}

// Helper function to append a uint16 value to the buffer
void ClientGroupDMCommands::append_uint16(std::vector<uint8_t>& buffer, uint16_t value) {
    buffer.push_back(static_cast<uint8_t>(value >> 8));
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

// Helper function to append a string to the buffer
void ClientGroupDMCommands::append_string(std::vector<uint8_t>& buffer, const std::string& str) {
    for (char c : str) {
        buffer.push_back(static_cast<uint8_t>(c));
    }
}
