#pragma once

class ResourcesManager
{
public:
	static char *_dir;

public:
	static char *loadTextFile(const char* fileName);
	static char *getFullPath(const char* fileName);
};

class FilesManager
{

public:
	static char* loadTextFile(const char* fileName);
};

