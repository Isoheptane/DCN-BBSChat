#include "pch.h"
#include "ClientFileCommands.h"
#include "Buffer.hpp"

using std::string;
using std::vector;

ClientFileCommand::ClientFileCommand(string commandType, string fileName, vector<uint8_t> fileData)
    : commandType(std::move(commandType)), fileName(std::move(fileName)), fileData(std::move(fileData)) {
}

ClientFileCommand ClientFileCommand::fileDownloadCommand(const string& fileName) {
    return ClientFileCommand("file_download", fileName);
}

ClientFileCommand ClientFileCommand::fileUploadCommand(const string& fileName, const vector<uint8_t>& fileData) {
    return ClientFileCommand("file_upload", fileName, fileData);
}

ClientFileCommand ClientFileCommand::fileDeleteCommand(const string& fileName) {
    return ClientFileCommand("file_delete", fileName);
}

vector<uint8_t> ClientFileCommand::toPacket() const {
    vector<uint8_t> buffer;

    // Command type
    append_uint8(buffer, static_cast<uint8_t>(commandType.size()));
    append_vector(buffer, commandType);

    // File name
    append_uint16(buffer, static_cast<uint16_t>(fileName.size()));
    append_vector(buffer, fileName);

    // If the command is for file upload, include file data
    if (commandType == "file_upload") {
        append_uint16(buffer, static_cast<uint16_t>(fileData.size()));
        append_vector(buffer, this->fileData);
    }

    return buffer;
}
