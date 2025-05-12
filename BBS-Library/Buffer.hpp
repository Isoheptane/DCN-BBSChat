#pragma once

#include <vector>
#include <string>

inline void append_vector(std::vector<uint8_t>& vector, uint8_t* array, size_t count) {
	for (size_t i = 0; i < count; i++) {
		vector.push_back(array[i]);
	}
}

inline void append_vector(std::vector<uint8_t>& vector, std::vector<uint8_t>& other) {
	vector.insert(vector.end(), other.begin(), other.end());
}

inline void append_vector(std::vector<uint8_t>& vector, std::string other) {
	append_vector(vector, (uint8_t*)other.c_str(), other.size());
}

inline void append_uint8(std::vector<uint8_t>& vector, uint8_t value) {
	vector.push_back(value);
}

inline void append_uint16(std::vector<uint8_t>& vector, uint16_t value) {
	vector.push_back(value >> 8 & 0xFF);
	vector.push_back(value >> 0 & 0xFF);
}

inline std::string take_string(const std::vector<uint8_t>& vector, size_t start, size_t count) {
	std::string str;
	for (size_t i = 0; i < count && start + i < vector.size(); i++) {
		str.push_back(vector[start + i]);
	}
	return str;
}

inline std::vector<uint8_t> take_bytes(const std::vector<uint8_t>& vector, size_t start, size_t count) {
	std::vector<uint8_t> buf;
	for (size_t i = 0; i < count && start + i < vector.size(); i++) {
		buf.push_back(vector[start + i]);
	}
	return buf;
}

inline uint8_t take_uint8(const std::vector<uint8_t>& vector, size_t position) {
	if (position >= vector.size()) {
		return 0;
	}
	return vector[position];
}

inline uint16_t take_uint16(const std::vector<uint8_t>& vector, size_t position) {
	if (position + 1 >= vector.size()) {
		return 0;
	}
	return vector[position] * 256 + vector[position + 1];
}