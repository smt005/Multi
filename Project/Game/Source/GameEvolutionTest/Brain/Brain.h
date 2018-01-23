#pragma once

class Neuron;

class Brain
{
private:
	unsigned int _count = 0;
	Neuron* _neurons = nullptr;

public:
	void action();

public:
	static void make(const Brain& brain);
};