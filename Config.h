#pragma once
#include <vector>
#include <string>

bool InitConfig();

int GetSearchMaxDepth();
int GetDelayStart();
std::vector<std::string>& GetFileExts();
unsigned long long GetFileSizeLimit();
std::string& GetSaveDir();