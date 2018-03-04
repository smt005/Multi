
#include "Neuron.h"

Neuron::Neuron()
{

}

Neuron::~Neuron()
{
}

void Neuron::input(float data)
{
	_data = data;
}

void Neuron::output()
{
	for (int i = 0; i < _count; ++i)
	{
		_neurons[i].input(_data);
	}
}

//---

Brain::Brain()
{

}

Brain::~Brain()
{
	delete _neurons;
}

void Brain::action()
{
	_neurons[_index].output();

	++_index;
	if (_index >= _count) _index = 0;
}

void Brain::generateNeuronÑonnections()
{

}
