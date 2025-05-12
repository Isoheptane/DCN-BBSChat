#include "pch.h"
#include "ClientGroupDMCommands.h"
#include "Buffer.hpp"

using std::string;
using std::vector;

// Constructor for group or DM command
ClientGroupDMCommands::ClientGroupDMCommands(std::string command, std::string name)
    : command(std::move(command)), name(std::move(name)) {
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
    return ClientGroupDMCommands("join_dm", username);
}

// Serialize the command into a protocol-compliant packet
vector<uint8_t> ClientGroupDMCommands::toPacket() const {
    vector<uint8_t> buffer;

    // Append command length and command string
    append_uint8(buffer, static_cast<uint8_t>(command.size()));
    append_vector(buffer, command);

	append_uint8(buffer, static_cast<uint8_t>(name.size()));
	append_vector(buffer, name);

    return buffer;
}

ClientGroupDMCommands ClientGroupDMCommands::fromPacket(std::vector<uint8_t> packet) {

	size_t counter = 0;
	size_t cml = take_uint8(packet, counter++);
	string command = take_string(packet, counter, cml);
	counter += cml;

	size_t nameLen = take_uint8(packet, counter++);
	string name = take_string(packet, counter, nameLen);
	counter += nameLen;

	return ClientGroupDMCommands(command, name);
}
