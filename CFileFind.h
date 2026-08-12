#ifndef CFILEFIND_H
#define CFILEFIND_H

#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

class CFileFind
{
public:
	CFileFind() { dir = NULL; };
	~CFileFind() { if (dir != NULL) closedir(dir); dir=NULL; }
	bool FindFile(const char *path)
	{
		dir = opendir(path);
		if (dir == nullptr) return false;
		searchpath = std::string(path);
		return true;
	}
	bool FindNextFile()
	{
		entry = readdir(dir);
		if (entry == nullptr) return false;
		return true;
	}
	bool IsDots()
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..") return true;
		return false;
	}
	bool IsDirectory()
	{
		return entry->d_type == DT_DIR;
	}
	std::string GetFileName()
	{
		return entry->d_name;	
	}
	std::string GetFilePath()
	{
		return searchpath;
	}
private:
	DIR* dir;
	std::string searchpath;
	struct dirent* entry;
};


#endif // CFILEFIND_H
