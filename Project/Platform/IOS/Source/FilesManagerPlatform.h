#pragma once

class FilesManagerPlatform
{
private:
	static char *_dir;

public:
	static char* loadTextFile(char* fileName);
	static char* getFullPath(char* fileName);
};
