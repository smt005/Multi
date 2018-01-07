#pragma once

#include "FilesManager.h"

class FilesManagerPlatform
{
private:
	static char *_dir;

public:
	static char* loadTextFile(const char* fileName);
	static char* getFullPath(const char* fileName);
};
