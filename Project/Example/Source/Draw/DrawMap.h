#pragma once

#include "Draw.h"
#include "Engine/Source/Callback/Callback.h"

class Object;

class DrawMap : public Draw, public Callback
{
private:
	unsigned int _program;
	unsigned int _cuttrentBufer = 0;
	unsigned int _cuttrentTexture = 0;

public:
	DrawMap();
	~DrawMap();

	void init();
	void prepareDraw(bool clear);
	void draw(bool clear);
	void drawModel( Object &object);

private:
	bool closeGame(void *data);
	bool rotateCamera(void *data);

};