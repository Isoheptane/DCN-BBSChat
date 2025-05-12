#pragma once

#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <mutex>

class WritingFile
{
private:
	std::ofstream file;
	time_t last_access;
	std::recursive_mutex mutex;
public:

	WritingFile(std::string filename);

	~WritingFile();

	bool appendBytes(std::vector<uint8_t> bytes);

	bool shouldClose(int timeout);
	
};

class WritingFilePool {
private:
	std::map<std::string, std::shared_ptr<WritingFile>> openedFiles;
	std::recursive_mutex mutex;

public:

	void closeAllTimedout(int timeout);

	std::shared_ptr<WritingFile> emplaceFile(std::string name);
	bool appendBytes(std::string name, const std::vector<uint8_t> bytes);
};