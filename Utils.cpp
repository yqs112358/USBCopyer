#include <Windows.h>
#include "Utils.h"
#include <ctime>
using namespace std;

char GetFirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 1)
            break;
        unitmask >>= 1;
    }
    return (i + 'A');
}

std::vector<std::string> SplitStrWithPattern(const std::string& str, const std::string& pattern)
{
    std::vector<std::string> resVec;

    if (str.empty())
        return resVec;

    std::string strs = str + pattern;

    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != std::string::npos) {
        std::string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(pattern);
    }

    return resVec;
}

bool StartsWith(const std::string& str, const std::string& start) {
    size_t srcLen = str.size();
    size_t startLen = start.size();
    if (srcLen >= startLen) {
        string temp = str.substr(0, startLen);
        if (temp == start)
            return true;
    }

    return false;
}

bool EndsWith(const std::string& str, const std::string& end) {
    size_t srcLen = str.size();
    size_t endLen = end.size();
    if (srcLen >= endLen) {
        string temp = str.substr(srcLen - endLen, endLen);
        if (temp == end)
            return true;
    }

    return false;
}

string& ReplaceStr(string& str, const string& old_value, const string& new_value) {
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
    return str;
}

string GetDeviceLabel(char drive)
{
    DWORD dwVolumeSerialNumber;
    DWORD dwMaximumComponentLength;
    DWORD dwFileSystemFlags;
    CHAR szFileSystemNameBuffer[1024];
    CHAR szDriveName[MAX_PATH];
    string drivePath = drive + ":\\";

    if (!GetVolumeInformationA(drivePath.c_str(), szDriveName, MAX_PATH, &dwVolumeSerialNumber,
        &dwMaximumComponentLength, &dwFileSystemFlags, szFileSystemNameBuffer, sizeof(szFileSystemNameBuffer)))
    {
        return "";
    }
    return string(szDriveName);
}

string GetDateString()
{
    time_t t = time(NULL);
    tm ts;
    localtime_s(&ts, &t);
    char buf[24] = { 0 };
    strftime(buf, 24, "%Y-%m-%d", &ts);
    return string(buf);
}

string GetTimeString()
{
    time_t t = time(NULL);
    tm ts;
    localtime_s(&ts, &t);
    char buf[24] = { 0 };
    strftime(buf, 24, "%H-%M-%S", &ts);
    return string(buf);
}