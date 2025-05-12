#include "pch.h"
#include "FileCache.h"

WritingFile::WritingFile(std::string filename) {
	this->last_access = time(NULL);
	this->file = std::ofstream(filename, std::ofstream::binary | std::ofstream::app);
}

WritingFile::~WritingFile() {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
}

bool WritingFile::appendBytes(std::vector<uint8_t> bytes) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	if (this->file.good()) {
		this->file.write((char*)bytes.data(), bytes.size());
		this->last_access = time(NULL);
		return true;
	}
	return false;
}

bool WritingFile::shouldClose(int timeout) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	time_t now = time(NULL);
	return this->last_access + timeout < now;
}

void WritingFilePool::closeAllTimedout(int timeout) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	std::vector<std::string> need_removal;

	for (auto it : this->openedFiles) {
		if (it.second.get()->shouldClose(timeout))
			need_removal.push_back(it.first);
	}

	for (auto name : need_removal) {
		this->openedFiles.erase(name);
		printf("[LOG] File %s closed\n", name.c_str());
	}
}

std::shared_ptr<WritingFile> WritingFilePool::emplaceFile(std::string name) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	auto it = this->openedFiles.find(name);

	// Not fount, emplace
	if (it == this->openedFiles.end()) {
		std::shared_ptr<WritingFile> file = std::make_shared<WritingFile>(name);
		this->openedFiles[name] = file;
		return file;
	}
	// Found
	return (*it).second;
}

bool WritingFilePool::appendBytes(std::string name, const std::vector<uint8_t> bytes) {
	auto file = this->emplaceFile(name);
	return file.get()->appendBytes(bytes);
}