#pragma once
#ifndef CLIENTGROUPDMCOMMANDS_H
#define CLIENTGROUPDMCOMMANDS_H

#include <string>
#include <vector>

class ClientGroupDMCommands {
public:
    // Constructor for joining or creating a group or DM
    ClientGroupDMCommands(std::string command, std::string name = "");

    // Static methods to create specific commands
    static ClientGroupDMCommands createGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands joinGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands removeGroupCommand(const std::string& groupName);
    static ClientGroupDMCommands joinDMCommand(const std::string& username);

    // Serialize the command into a protocol-compliant packet
    std::vector<uint8_t> toPacket() const;
    static ClientGroupDMCommands fromPacket(std::vector<uint8_t>);

    std::string command;
    std::string name;
};

#endif // CLIENTGROUPDMCOMMANDS_H
