#pragma once

#include "Draw.h"

class DrawGreed : public Draw
{
private:
	unsigned int _program;
	int _u_matrix;
	int _u_pointSize;
	int _u_color;

	float _colorRed[4];
	float _colorBlue[4];
	float _colorGreen[4];

	float _colorBlackContrast[4];
	float _colorRedContrast[4];
	float _colorBlueContrast[4];
	float _colorGreenContrast[4];

	void prepareDraw();
	void drawLine(float* line);
	void drawPoint(float* point);

public:
	DrawGreed();
	~DrawGreed();

	void draw();
	void drawGreed(bool clear = true);

private:
	static DrawGreed *_pInstance;

public:
	static void draw(bool clear = true);
	static void clear();
};