#pragma once

class Neuron
{
public:
	Neuron();
	~Neuron();

	void input(float data);
	void output();

private:
	float _data = 0.0f;
	unsigned long int _count = 0;
	Neuron* _neurons = nullptr;
};


class Brain
{
public:
	Brain();
	~Brain();

	void action();
	void generateNeuron—onnections();

private:
	unsigned long int _count = 0;
	Neuron* _neurons = nullptr;

	unsigned long int _index = 0;
};
