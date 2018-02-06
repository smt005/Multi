#pragma once

class ApplicationPlatform
{
public:

public:
    static bool execution();
    
public:
	static void actionOnFrame();

	static int width();
	static int height();
	static float aspect();
	static double getCurentTime();
	static void close();

private:
    static int _windowWidth;
    static int _windowHeight;
    static float _cursorPos[2];
};
