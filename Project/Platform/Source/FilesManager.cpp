
#include "FilesManager.h"
#include "FilesManagerPlatform.h"

char* FilesManager::loadTextFile(const char* fileName)
{
	return FilesManagerPlatform::loadTextFile(fileName);
}

void FilesManager::saveTextFile(const char * fileName, const char * data)
{
#ifdef WIN32
	FilesManagerPlatform::saveTextFile(fileName, data);
#endif

}

char* FilesManager::getFullPath(const char* fileName)
{
	return FilesManagerPlatform::getFullPath(fileName);
}
