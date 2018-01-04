#pragma once

#include "Draw.h"

class DrawLoadOBJ : public Draw
{
private:
	unsigned int _program;
	unsigned int _textureId;

	int _countIndex;
	unsigned int _buffer[3];

	int initVBO();
	int initModelVBO();
	void getGridTextureId();

	int _time;
	int _typeBuffer;

public:
	DrawLoadOBJ();
	~DrawLoadOBJ();

	void draw(bool clear = true);
};