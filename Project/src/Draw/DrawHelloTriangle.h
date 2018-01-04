#pragma once

#include "Draw.h"

class DrawHelloTriangle: public Draw
{
private:
	unsigned int _program;

public:
	DrawHelloTriangle();
	~DrawHelloTriangle();

	void draw(bool clear = true);
};