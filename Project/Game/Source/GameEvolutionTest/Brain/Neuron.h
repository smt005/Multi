#pragma once

enum {
	NONE,
	NEXT,
	ADDITION,
	MULTIPLICATION,
	MEMORY
};

class Neuron
{
private:
	bool _value = 0;
	unsigned int _count = 0;
	Neuron* _neurons = nullptr;

public:
	void action(const bool& value);

public:
	static Neuron* make(Neuron* parentNeuron = nullptr);
};