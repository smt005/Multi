
#include "FilesManagerPlatform.h"
#include <fstream>
#include <iostream>

char *FilesManagerPlatform::_dir = WIN32_RESOURCES_DIR;

char* FilesManagerPlatform::loadTextFile(const char* fileName)
{
	if (!fileName)
	{
		return NULL;
	}

	char *fileNameWithDir = getFullPath(fileName);
	
	FILE* file;
	char* buf = NULL;

	if (fopen_s(&file, fileNameWithDir, "r") == NULL)
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

void FilesManagerPlatform::saveTextFile(const char *fileName, const char *data)
{
	if (!fileName)
	{
		return;
	}

	char *fileNameWithDir = getFullPath(fileName);

	FILE* file;
	if (fopen_s(&file, fileNameWithDir, "w") != NULL)
	{
		delete[] fileNameWithDir;
		return;
	}

	fputs(data, file);

	fclose(file);
	delete[] fileNameWithDir;
}

char* FilesManagerPlatform::getFullPath(const char* fileName)
{
	int lenDir = strlen(_dir);
	int lenName = strlen(fileName);
	int sumLen = lenDir + lenName + 1;

	char* fileNameWithDir = new char[sumLen];
	strcpy_s(fileNameWithDir, lenDir + 1, _dir);
	strncat(fileNameWithDir, fileName, lenName);

	return fileNameWithDir;
}
