#pragma once
#ifndef CLIENTGROUPDMCOMMANDS_H
#define CLIENTGROUPDMCOMMANDS_H

#include <string>
#include <vector>

class ClientGroupDMCommands {
public:
    // Constructor for joining or creating a group or DM
    ClientGroupDMCommands(std::string command, std::string groupName = "", std::string username = "");

    // Static methods to create specific commands
    static ClientGroupDMCommands createGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands joinGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands removeGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands joinDMCommand(const std::string& username);

    // Serialize the command into a protocol-compliant packet
    std::vector<uint8_t> toPacket() const;

private:
    std::string command;
    std::string groupName;
    std::string username;

    // Helper functions for appending data to the packet
    static void append_uint8(std::vector<uint8_t>& buffer, uint8_t value);
    static void append_uint16(std::vector<uint8_t>& buffer, uint16_t value);
    static void append_string(std::vector<uint8_t>& buffer, const std::string& str);
};

#endif // CLIENTGROUPDMCOMMANDS_H
