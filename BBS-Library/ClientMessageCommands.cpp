#include "pch.h"
#include "ClientMessageCommands.h"
#include "Buffer.hpp"

using std::string;
using std::vector;

ClientMessage::ClientMessage(string type, string content)
    : type(std::move(type)), content(std::move(content)) {
}

ClientMessage ClientMessage::textMessage(const string& content) {
    return ClientMessage("text", content);
}

ClientMessage ClientMessage::fileHintMessage(const string& content) {
    return ClientMessage("file_hint", content);
}

vector<uint8_t> ClientMessage::toPacket() {
    vector<uint8_t> buffer;
    const static string COMMAND = "message";

    append_uint8(buffer, static_cast<uint8_t>(COMMAND.size()));
    append_vector(buffer, COMMAND);

    append_uint8(buffer, static_cast<uint8_t>(type.size()));
    append_vector(buffer, type);

    append_uint16(buffer, static_cast<uint16_t>(content.size()));
    append_vector(buffer, content);

    return buffer;
}

ClientMessage ClientMessage::fromPacket(vector<uint8_t> packet) {

    size_t counter = 0;
    size_t cml = take_uint8(packet, counter++);
    counter += cml;

    size_t typeLen = take_uint8(packet, counter++);
    string type = take_string(packet, counter, typeLen);
    counter += typeLen;

    size_t contentLen = take_uint16(packet, counter);
    counter += 2;
    string content = take_string(packet, counter, contentLen);
    counter += contentLen;

    return ClientMessage(type, content);
}
