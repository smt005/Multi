#pragma once

#include <string>

using namespace std;

template <class ArrayT>

class ArrayTemplate
{
private:
	unsigned int _increaseCapasity = 100;	//10;
	unsigned int _capasity = 0;
	unsigned int _count = 0;
	ArrayT **_array = 0;

public:

	ArrayTemplate() {};
	~ArrayTemplate() { clean(); };

	unsigned int count() { return _count; };

	int getIndex(const string &name);
	int getIndex(const ArrayT &item);

	void clean();
	void remove(const string &name);
	void remove(const ArrayT &item);
	void remove(const int &index);

	ArrayT* addReturnPoint();
	ArrayT& add();
	ArrayT& add(ArrayT *newItem);

	inline ArrayT& operator[](const string &name) { return getByName(name); };
	inline ArrayT& operator[](const unsigned int &index) { return getByIndex(index); };
	ArrayT& getByName(const string &name);
	ArrayT& getByIndex(const unsigned int &index);

};

template <class ArrayT>
int ArrayTemplate<ArrayT>::getIndex(const string &searchName)
{
	if (!_array) return;

	for (int i = 0; i < _count; ++i)
	{
		ArrayT *item = _array[i];

		if (item)
		{
			if (item->name() == searchName) return i;
		}
	}

	return -1;
}

template <class ArrayT>
int ArrayTemplate<ArrayT>::getIndex(const ArrayT &searchItem)
{
	if (!_array) return;

	for (int i = 0; i < _count; ++i)
	{
		ArrayT *item = _array[i];

		if (item)
		{
			if (item == &searchItem) return i;
		}
	}

	return -1;
}

//---

template <class ArrayT>
void ArrayTemplate<ArrayT>::clean()
{
	if (!_array)
	{
		_array = 0;
		_capasity = 0;
		_count = 0;
		return;
	}

	for (unsigned int i = 0; i < _count; ++i)
	{
		if (!_array[i]) delete _array[i];
	}

	delete[] _array;
	_array = 0;
	_capasity = 0;
	_count = 0;
}

template <class ArrayT>
void ArrayTemplate<ArrayT>::remove(const string &name)
{
	if (!_array) return;

	int index = getIndex(name);
	if (index >= 0 && index < _count)
		remove(index);
}

template <class ArrayT>
void ArrayTemplate<ArrayT>::remove(const ArrayT &item)
{
	if (!_array) return;

	int index = getIndex(item);
	if (index >= 0 && index < _count)
		remove(index);
}

template <class ArrayT>
void ArrayTemplate<ArrayT>::remove(const int &index)
{
	if (!_array || (index == 0 && index >= _count)) return;

	_capasity = _count + _increaseCapasity;
	ArrayT* *newArray = new ArrayT*[_capasity];

	delete[] _array[index];
	_array[index] = 0;

	for (static unsigned int i = 0; i < _count; ++i)
	{
		ArrayT *item = _array[i];
		if (item) newArray[i] = item;
	}

	delete[] _array;
	_array = newArray;

	--_count;
}

//---

template <class ArrayT>
ArrayT* ArrayTemplate<ArrayT>::addReturnPoint()
{
	ArrayT *newItem = new ArrayT();

	if (!_array)
	{
		_array = new ArrayT*[100];
		_count = 0;
	}

	_array[_count] = newItem;
	++_count;
	return newItem;
}

template <class ArrayT>
ArrayT& ArrayTemplate<ArrayT>::add()
{
	ArrayT *newItem = new ArrayT();
	return add(newItem);
}

template <class ArrayT>
ArrayT& ArrayTemplate<ArrayT>::add(ArrayT *newItem)
{
	if (!newItem)
	{
		if (_count > 0)
		{
			ArrayT *item = _array[0];
			return *item;
		}

		newItem = new ArrayT();
	}

	if (_count >= _capasity)
	{
		_capasity = _capasity + _increaseCapasity;
		ArrayT* *newArray = new ArrayT*[_capasity];

		for (static unsigned int i = 0; i < _count; ++i) newArray[i] = _array[i];

		delete[] _array;
		_array = newArray;
	}

	_array[_count] = newItem;
	++_count;

	return *newItem;
}

//---

template <class ArrayT>
ArrayT& ArrayTemplate<ArrayT>::getByName(const string &name)
{
	if (_array)
	{
		for (int i = 0; i < _count; ++i)
		{
			ArrayT *item = _array[i];
			if (item->name() == name) return *item;
		}
	}

	ArrayT *newItem = new ArrayT();
	return add(newItem);
}

template <class ArrayT>
ArrayT& ArrayTemplate<ArrayT>::getByIndex(const unsigned int &index)
{
	if (index >= 0 && index < _count)
	{
		ArrayT *item = _array[index];
		return *item;
	}

	ArrayT *newItem = new ArrayT();
	return add(newItem);
}
