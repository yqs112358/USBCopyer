#pragma once
#include <string>

struct CopyInfoStruct
{
	std::string fromDir;
	std::string targetDir;
	bool* exitSign;
};

unsigned int StartCopy(void* info);