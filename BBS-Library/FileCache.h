#pragma once

#include <ctime>
#include <vector>
#include <string>
#include <fstream>

class WritingFile
{
private:
	std::ofstream file;
	time_t last_access;

	WritingFile(std::string filename);

	void appendBytes(std::vector<uint8_t> bytes);

	bool shouldClose(time_t timeout);
	
};

