#pragma once

class FilesManager
{
public:
	static char* loadTextFile(const char* fileName);
	static void saveTextFile(const char* fileName, const char* data);
	static char* getFullPath(const char* fileName);
};

