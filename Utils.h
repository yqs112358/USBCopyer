#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using std::string;

char GetFirstDriveFromMask(ULONG unitmask);
std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern);
bool StartsWith(const std::string& str, const std::string& start);
bool EndsWith(const std::string& str, const std::string& end);
std::string& ReplaceStr(std::string& str, const std::string& old_value, const std::string& new_value);
string GetDeviceLabel(char drive);
string GetDateString();
string GetTimeString();