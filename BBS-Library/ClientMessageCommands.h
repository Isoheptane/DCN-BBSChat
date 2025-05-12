#pragma once
#ifndef CLIENTMESSAGE_H
#define CLIENTMESSAGE_H

#include <string>
#include <vector>

class ClientMessage {
public:
    // Constructor to initialize message type and content
    ClientMessage(std::string type, std::string content);

    // Static constructors for different message types
    static ClientMessage textMessage(const std::string& content);
    static ClientMessage fileHintMessage(const std::string& content);

    // Serialize the message to a protocol-compliant packet
    std::vector<uint8_t> toPacket() const;

private:
    std::string type;     // Type of the message (e.g., "text", "file_hint")
    std::string content;  // The content of the message (e.g., text content, file hint)
};

#endif // CLIENTMESSAGE_H
