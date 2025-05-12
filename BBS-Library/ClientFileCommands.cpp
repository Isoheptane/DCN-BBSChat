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
    append_string(buffer, commandType);

    // File name
    append_uint16(buffer, static_cast<uint16_t>(fileName.size()));
    append_string(buffer, fileName);

    // If the command is for file upload, include file data
    if (commandType == "file_upload") {
        append_uint16(buffer, static_cast<uint16_t>(fileData.size()));
        append_vector(buffer, fileData);
    }

    return buffer;
}

void ClientFileCommand::append_uint8(vector<uint8_t>& buffer, uint8_t value) {
    buffer.push_back(value);
}

void ClientFileCommand::append_uint16(vector<uint8_t>& buffer, uint16_t value) {
    buffer.push_back(static_cast<uint8_t>(value >> 8));
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
}

void ClientFileCommand::append_string(vector<uint8_t>& buffer, const string& str) {
    for (char c : str) {
        buffer.push_back(static_cast<uint8_t>(c));
    }
}

void ClientFileCommand::append_vector(vector<uint8_t>& buffer, const vector<uint8_t>& data) {
    buffer.insert(buffer.end(), data.begin(), data.end());
}
