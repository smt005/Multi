
#include "FilesManager.h"
#include "FilesManagerPlatform.h"

char* FilesManager::loadTextFile(const char* fileName)
{
	return FilesManagerPlatform::loadTextFile(fileName);
}

char* FilesManager::getFullPath(const char* fileName)
{
	return FilesManagerPlatform::getFullPath(fileName);
}
