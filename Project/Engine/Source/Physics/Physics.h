#pragma once

class Physics
{
private:

public:
	Physics();
	~Physics();

	bool init();
	void clean();

	void createWorldTest();
	void update();
};
