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

vector<uint8_t> ClientFileCommand::toPacket() {
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

ClientFileCommand ClientFileCommand::fromPacket(std::vector<uint8_t> packet) {

    size_t counter = 0;
    size_t cml = take_uint8(packet, counter++);
    string type = take_string(packet, counter, cml);
    counter += cml;

    size_t nameLen = take_uint8(packet, counter++);
    string name = take_string(packet, counter, nameLen);
    counter += nameLen;

    std::vector<uint8_t> content;

    // Extra segement of file_upload
    if (type == "file_upload") {
        size_t contentLen = take_uint16(packet, counter);
        counter += 2;
        content = take_bytes(packet, counter, contentLen);
        if (contentLen != content.size()) {
            printf("[ERROR] File upload packet receive error: Length not match\n");

        }
    }

    return ClientFileCommand(type, name, content);
}