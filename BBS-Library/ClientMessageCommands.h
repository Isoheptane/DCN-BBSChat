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

    // Helper methods to append data to the packet
    static void append_uint8(std::vector<uint8_t>& buffer, uint8_t value);
    static void append_uint16(std::vector<uint8_t>& buffer, uint16_t value);
    static void append_vector(std::vector<uint8_t>& buffer, const std::string& str);
};

#endif // CLIENTMESSAGE_H
