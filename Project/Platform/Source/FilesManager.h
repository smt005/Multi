#pragma once

class FilesManager
{
public:
	static char* loadTextFile(const char* fileName);
	static char* getFullPath(const char* fileName);
};

