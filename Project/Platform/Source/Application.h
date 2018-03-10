#pragma once

class AppConfig
{
private:
	int _height = 500;
	int _width = 1000;
	bool _example = false;
	int _exampleNumber = 0;

public:
	int getHeight() { return _height;	};
	int getWidth() { return _width; };
	bool getExample() { return _example; };
	int getExampleNumber() { return _exampleNumber; };

public:
	void load();
	void save();
};

class App
{
public:
	static int width();
	static int height();
	static float aspect();
	static double getCurentTime();
	static void close();
};
