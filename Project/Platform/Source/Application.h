#pragma once

#include <string>

class AppConfig
{
private:
	int _height = 500;
	int _width = 1000;
	bool _example = false;
	int _exampleNumber = 0;

	std::string _nameGame = "GameTerrain";

public:
	int getHeight() { return _height;	};
	int getWidth() { return _width; };
	bool getExample() { return _example; };
	int getExampleNumber() { return _exampleNumber; };

	const std::string& getNameGame() { return _nameGame; };

public:
	void load();
	void save();
};

class App
{
public:
	static AppConfig& getAppConfig();
	static int width();
	static int height();
	static float aspect();
	static double getCurentTime();
	static void close();
};
