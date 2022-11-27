#include "Copyer.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "Config.h"
#include "Utils.h"
using namespace std;


// currentDir: Absolute path to the current directory to copy
// targetDir: Current target copy directory to paste
void CopyRecursively(const filesystem::path& currentDir, const filesystem::path& targetDir, int depth, bool* exitSign)
{
	int maxDepth = GetSearchMaxDepth();
	auto fileExts = GetFileExts();
	unsigned long long fileSizeLimit = GetFileSizeLimit();

	if (*exitSign)
		return;
	auto iter = filesystem::directory_iterator(currentDir);
	for (auto& it : iter)
	{
		try
		{
			if (*exitSign)
				return;
			auto fromPath = it.path();
			auto targetPath = targetDir / fromPath.filename();
			if (filesystem::is_directory(fromPath))
			{
				// dir
				// check depth
				if (maxDepth > 0 && depth >= maxDepth)	// cannot go deeper
					continue;

				if (*exitSign)
					return;
				if (!filesystem::create_directories(targetPath))
				{
					cout << "Fail to create copy target dir " << targetPath.u8string();
					continue;
				}
				if (*exitSign)
					return;

				// recursively
				printf("Go into dir %s\n", targetPath.u8string().c_str());
				CopyRecursively(fromPath, targetPath, depth + 1, exitSign);
				if (*exitSign)
					return;
			}
			else
			{
				// file
				bool extMatched = false;
				if (fileExts.empty())
					extMatched = true;
				else
				{
					for (auto& ext : fileExts)
						if (EndsWith(fromPath.u8string(), ext))
						{
							extMatched = true;
							break;
						}
				}
				bool fileSizeOk = (fileSizeLimit == 0 ? true : filesystem::file_size(fromPath) <= fileSizeLimit);

				if (*exitSign)
					return;
				if (extMatched && fileSizeOk)
				{
					printf("Copy %s -> %s\n", fromPath.u8string().c_str(), targetPath.u8string().c_str());
					filesystem::copy(fromPath, targetPath, filesystem::copy_options::overwrite_existing);
					if (*exitSign)
						return;
				}
			}
		}
		catch (const exception&)
		{
			if (*exitSign)
				return;
		}
	}
}

unsigned int StartCopy(void* info)
{
	// delay start
	int delay = GetDelayStart();
	if (delay > 0)
	{
		printf("Sleep %ds before start...\n", delay);
		Sleep(delay * 1000);
	}

	CopyInfoStruct* copyInfo = (CopyInfoStruct*)info;
	string fromDir = copyInfo->fromDir;
	string targetDir = copyInfo->targetDir;
	bool* exitSignal = copyInfo->exitSign;
	delete copyInfo;

	string label = GetDeviceLabel(fromDir[0]);
	if (label.empty())
		label = string("±æµÿ¥≈≈Ã") + fromDir[0];
	ReplaceStr(targetDir, "<drivelabel>", label);
	ReplaceStr(targetDir, "<date>", GetDateString());
	ReplaceStr(targetDir, "<time>", GetTimeString());

	// remove dir if needed
	std::error_code ec;
	if (filesystem::exists(targetDir, ec))
	{
		filesystem::remove_all(targetDir, ec);
	}

	printf("[INFO] Start copying from %s -> %s\n", fromDir.c_str(), targetDir.c_str());
	CopyRecursively(fromDir, targetDir, 1, exitSignal);
	printf("[INFO] Copy process finished.\n");
	return 0;
}