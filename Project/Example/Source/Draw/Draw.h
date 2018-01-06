#pragma once

class Draw
{
public:
	bool _status;

	Draw();
	~Draw();

	virtual void draw(bool clear = true);

	static void resetNumber();
	static void draws();
	static bool nextDraw(int step);
};
