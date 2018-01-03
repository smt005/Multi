#pragma once

class Draw
{
public:
	bool _status;

	Draw();
	~Draw();

	virtual void draw();

	static void resetNumber();
	static void draws();
	static bool nextDraw(int step);
};
