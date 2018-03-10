#pragma once

#include <string>

using namespace std;

template <class Tarray>

class ArrayClass
{
protected:
	string _name;

public:
	const string& name() { return _name; }
	void setName(const string &name) { _name = name; }
	virtual void create(const string &name);

// STATIC

private:
	static unsigned int _increaseCapasity;
	static unsigned int _capasity;
	static unsigned int _count;
	static Tarray **_array;

public:
	static unsigned int count() { return _count; };

	static int getIndex(const string &name);
	static int getIndex(const Tarray &item);

	static void clean();
	static void remove(const string &name);
	static void remove(const Tarray &item);
	static void remove(const int &index);

	static Tarray& add();
	static Tarray& add(Tarray *newItem);

	static Tarray& getByName(const string &name);
	static Tarray& getByIndex(const unsigned int &index);

};

// VIRTUAL 

template <class Tarray>
void ArrayClass<Tarray>::create(const string &name)
{
	setName(name);
}

// PUBLIC STATIC

template <class Tarray>
unsigned int ArrayClass<Tarray>::_increaseCapasity = 10;

template <class Tarray>
unsigned int ArrayClass<Tarray>::_capasity = 0;

template <class Tarray>
unsigned int ArrayClass<Tarray>::_count = 0;

template <class Tarray>
Tarray **ArrayClass<Tarray>::_array = 0;

template <class Tarray>
int ArrayClass<Tarray>::getIndex(const string &searchName)
{
	for (int i = 0; i < _count; ++i)
	{
		Tarray *item = _array[i];

		if (item)
		{
			if (item->_name == searchName) return i;
		}
	}

	return -1;
}

template <class Tarray>
int ArrayClass<Tarray>::getIndex(const Tarray &searchItem)
{
	for (int i = 0; i < _count; ++i)
	{
		Tarray *item = _array[i];

		if (item)
		{
			if (item == &searchItem) return i;
		}
	}

	return -1;
}

//---

template <class Tarray>
void ArrayClass<Tarray>::clean()
{
	for (int i = 0; i < _count; ++i)
	{
		delete _array[i];
	}

	delete[] _array;
	_array = 0;
	_capasity = 0;
	_count = 0;
}

template <class Tarray>
void ArrayClass<Tarray>::remove(const string &name)
{
	int index = getIndex(name);
	if (index >= 0 && index < _count)
		remove(index);
}

template <class Tarray>
void ArrayClass<Tarray>::remove(const Tarray &item)
{
	int index = getIndex(item);
	if (index >= 0 && index < _count)
		remove(index);
}

template <class Tarray>
void ArrayClass<Tarray>::remove(const int &index)
{
	if (index == 0 && index >= _count) return;

	_capasity = _count + _increaseCapasity;
	Tarray* *newArray = new Tarray*[_capasity];

	delete[] _array[index];
	_array[index] = 0;

	for (static unsigned int i = 0; i < _count; ++i)
	{
		Tarray *item = _array[i];
		if (item) newArray[i] = item;
	}

	delete[] _array;
	_array = newArray;

	--_count;
}

//---

template <class Tarray>
Tarray& ArrayClass<Tarray>::add()
{
	Tarray *newItem = new Tarray();
	return add(newItem);
}

template <class Tarray>
Tarray& ArrayClass<Tarray>::add(Tarray *newItem)
{
	if (!newItem)
	{
		if (_count > 0)
		{
			Tarray *item = _array[0];
			return *item;
		}

		newItem = new Tarray();
	}

	if (_count >= _capasity)
	{
		_capasity = _capasity + _increaseCapasity;
		Tarray* *newArray = new Tarray*[_capasity];

		for (static unsigned int i = 0; i < _count; ++i) newArray[i] = _array[i];

		delete[] _array;
		_array = newArray;
	}

	_array[_count] = newItem;
	++_count;

	return *newItem;
}

//---

template <class Tarray>
Tarray& ArrayClass<Tarray>::getByName(const string &name)
{
	for (int i = 0; i < _count; ++i)
	{
		Tarray *item = _array[i];
		if (item->_name == name) return *item;
	}

	Tarray *newItem = new Tarray();
	newItem->setName(name);
	newItem->create(name);
	return add(newItem);
}

template <class Tarray>
Tarray& ArrayClass<Tarray>::getByIndex(const unsigned int &index)
{
	if (index >= 0 && index < _count)
	{
		Tarray *item = _array[index];
		return *item;
	}

	Tarray *newItem = new Tarray();
	newItem->create("default");
	return add(newItem);
}
