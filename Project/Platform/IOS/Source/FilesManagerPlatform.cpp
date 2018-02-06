
#include "FilesManagerPlatform.h"
#include <fstream>
#include <iostream>

char *FilesManagerPlatform::_dir = IOS_RESOURCES_DIR;

char* FilesManagerPlatform::loadTextFile(const char* fileName)
{
	if (!fileName)
	{
		return NULL;
	}

    char* buf = NULL;
	char *fileNameWithDir = getFullPath(fileName);
    FILE *file = fopen(fileNameWithDir, "r");
    
    if(file)
	{
		delete[] fileNameWithDir;

		fseek(file, 0, SEEK_END);
		int lenght = ftell(file);
		rewind(file);

		if (lenght == 0)
		{
			fclose(file);
			return NULL;
		}

		buf = (char *)malloc(sizeof(char)* (lenght + 1));
		lenght = fread(buf, sizeof(char), lenght, file);
		buf[lenght] = '\0';
		
		fclose(file);
	}
	else
	{
		delete[] fileNameWithDir;
		return NULL;
	}

	return buf;
}

char* FilesManagerPlatform::getFullPath(const char* fileName)
{
	int lenDir = strlen(_dir);
	int lenName = strlen(fileName);
	int sumLen = lenDir + lenName + 1;

	char* fileNameWithDir = new char[sumLen];
	strcpy(fileNameWithDir, _dir);
	strncat(fileNameWithDir, fileName, lenName);

	return fileNameWithDir;
}

