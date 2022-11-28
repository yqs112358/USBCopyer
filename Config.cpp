#define CONFIG_FILE_PATH "config.ini"
#define CONFIG_FILE_DEF_CONTENT \
R"([Main]
SearchMaxDepth=5
DelayStart=30
FileExts=.doc|.ppt|.xls|.docx|.pptx|.xlsx|.txt|.pdf
FileSizeLimit=1000MB
SavePath=./.saved/<date>_<time>_<drivelabel>/
)"


#include "Config.h"
#include "SimpleIni.h"
#include <filesystem>
#include <fstream>
#include "Utils.h"
using namespace std;

CSimpleIniA ini;
int searchMaxDepth = 5;
int delayStart = 30;
std::vector<std::string> fileExts = {};
unsigned long long fileSizeLimit = 0;
std::string saveDir = "./.saved/<date>_<time>_<drivelabel>/";

bool InitConfig()
{
	// if not exist config, create & exit
	if (!filesystem::exists(CONFIG_FILE_PATH))
	{
		ofstream fout(CONFIG_FILE_PATH);
		fout << CONFIG_FILE_DEF_CONTENT;
		fout.close();
		return false;
	}

	// file exist
	ini.SetUnicode();
	SI_Error rc = ini.LoadFile(CONFIG_FILE_PATH);
	if (rc < 0)
		return false;

	// parse basic data
	searchMaxDepth = ini.GetLongValue("Main", "SearchMaxDepth", 0);
	delayStart = ini.GetLongValue("Main", "DelayStart", 0);
	
	// parse file exts
	string extsStr = ini.GetValue("Main", "FileExts", "");
	if (extsStr.empty())
		fileExts = {};
	else
		fileExts = SplitStrWithPattern(extsStr, "|");

	// parse file size string
	string sizeLimitStr = ini.GetValue("Main", "FileSizeLimit", "0");
	string realNumStr;
	int ratio;
	if (EndsWith(sizeLimitStr, "GB"))
	{
		realNumStr = sizeLimitStr.substr(0, sizeLimitStr.size() - 2);
		ratio = 30;
	}
	else if (EndsWith(sizeLimitStr, "MB"))
	{
		realNumStr = sizeLimitStr.substr(0, sizeLimitStr.size() - 2);
		ratio = 20;
	}
	else if (EndsWith(sizeLimitStr, "KB"))
	{
		realNumStr = sizeLimitStr.substr(0, sizeLimitStr.size() - 2);
		ratio = 10;
	}
	else if (EndsWith(sizeLimitStr, "B"))
	{
		realNumStr = sizeLimitStr.substr(0, sizeLimitStr.size() - 1);
		ratio = 0;
	}
	else
	{
		// default as B
		realNumStr = sizeLimitStr;
		ratio = 0;
	}
	unsigned long long num = stoi(realNumStr);
	fileSizeLimit = num << ratio;

	// parse save dir
	saveDir = ini.GetValue("Main", "SavePath", "./.saved/<date>_<time>_<drivelabel>/");
	if (saveDir.find(":") == string::npos)
	{
		// change saveDir to real absolute path
		saveDir = (filesystem::current_path() / saveDir).lexically_normal().u8string();
	}
	else
		saveDir = filesystem::path(saveDir).lexically_normal().u8string();
	printf("[INFO] Saving to %s\n", saveDir.c_str());

	// success
	printf("[INFO] Config data loaded.\n");
	return true;
}

int GetSearchMaxDepth()
{
	return searchMaxDepth;
}

int GetDelayStart()
{
	return delayStart;
}

std::vector<std::string>& GetFileExts()
{
	return fileExts;
}

unsigned long long GetFileSizeLimit()
{
	return fileSizeLimit;
}

std::string& GetSaveDir()
{
	return saveDir;
}