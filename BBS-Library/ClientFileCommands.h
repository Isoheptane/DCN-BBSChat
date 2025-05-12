#pragma once
#ifndef CLIENTFILECOMMANDS_H
#define CLIENTFILECOMMANDS_H

#include <string>
#include <vector>

class ClientFileCommand {
public:
    // Constructor for file commands
    ClientFileCommand(std::string commandType, std::string fileName = "", std::vector<uint8_t> fileData = {});

    // Static method to create a file download command
    static ClientFileCommand fileDownloadCommand(const std::string& fileName);

    // Static method to create a file upload command
    static ClientFileCommand fileUploadCommand(const std::string& fileName, const std::vector<uint8_t>& fileData);

    // Static method to create a file delete command
    static ClientFileCommand fileDeleteCommand(const std::string& fileName);

    // Serialize command into a protocol-compliant packet
    std::vector<uint8_t> toPacket() const;

private:
    std::string commandType;    // Command type, e.g., "file_download", "file_upload", "file_delete"
    std::string fileName;       // File name (relevant for all file commands)
    std::vector<uint8_t> fileData; // File data (relevant for file upload)

};

#endif // CLIENTFILECOMMANDS_H
