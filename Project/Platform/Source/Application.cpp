
#include "Application.h"
#include "ApplicationPlatform.h"

#include "../Platform/Source/FilesManager.h"
#include "Engine/Source/Common/json.h"
#include <string>

using namespace std;
using json = nlohmann::json;

void AppConfig::load()
{
	string fileName = "Data/Setting.json";
	string dataString = FilesManager::loadTextFile(fileName.c_str());;
	json data = json::parse(dataString.c_str());

	/*
	{
		"width": 1920,
		"height": 1080,
		"game":
		{
			"name": "GameMini"
		},
		"example":
		{
			"state": false,
			"number": 5
		}
	}
	*/

	_height = data["height"].is_number_integer() ? data["height"].get<int>() : 480;
	_width = data["width"].is_number_integer() ? data["width"].get<int>() : 640;

	if (!data["example"].is_null())
	{
		_example = data["example"]["state"].is_boolean() ? data["example"]["state"].get<bool>() : false;
		_exampleNumber = data["example"]["number"].is_number_integer() ? data["example"]["number"].get<int>() : 0;
	}

	if (!data["game"].is_null())
	{
		const string& nameGame = data["game"]["name"].is_string() ? data["game"]["name"] : "Game";
		_nameGame = nameGame;
	}
}

void AppConfig::save()
{

}

//---
AppConfig& App::getAppConfig()
{
	return ApplicationPlatform::getAppConfig();
}

int App::width()
{
	return ApplicationPlatform::width();
}

int App::height()
{
	return ApplicationPlatform::height();
}

float App::aspect()
{
	return ApplicationPlatform::aspect();
}

double App::getCurentTime()
{
	return ApplicationPlatform::getCurentTime();
}

void App::close()
{
	return ApplicationPlatform::close();
}
