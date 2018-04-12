#pragma once

#include <string>
#include <map>
#include <memory>

using namespace std;

template <class ObjectT>
#define ObjectPtrT shared_ptr<ObjectT>

class DataClass
{
protected:
	string _name;

public:
	DataClass() {};
	virtual ~DataClass() {};

	virtual bool create(const string &name);

	void setName(const string& name) { _name = name; };
	const string name() { return _name; };

private:
	static map<string, ObjectPtrT> _map;
	static ObjectPtrT _defaultPtr;

public:
	static ObjectPtrT& getByName(const string& name);
	static bool hasByName(const string& name);

	static void remove(const string& name);
	static void clear();
};

template <class ObjectT>
map<string, ObjectPtrT> DataClass<ObjectT>::_map;

template <class ObjectT>
ObjectPtrT DataClass<ObjectT>::_defaultPtr;

template <class ObjectT>
bool DataClass<ObjectT>::create(const string &name)
{
	_name = name;
	return true;
};

template <class ObjectT>
ObjectPtrT& DataClass<ObjectT>::getByName(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
	{
		return it->second;
	}

	ObjectT* newItem = new ObjectT();
	if (newItem->create(name))
	{
		ObjectPtrT newItemPtr(newItem);
		return _map[name] = newItemPtr;
	}

	if (!_defaultPtr)
	{
		_defaultPtr = make_shared<ObjectT>();
		_defaultPtr->setName("NONE_NAME");
	}

	return _defaultPtr;
}

template <class ObjectT>
bool DataClass<ObjectT>::hasByName(const string& name)
{
	auto it = _map.find(name);
	return it != _map.end() ? true : false;
}

template <class ObjectT>
void DataClass<ObjectT>::remove(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
	{
		_map.erase(it);
	}
}

template <class ObjectT>
void DataClass<ObjectT>::clear()
{
	_map.clear();
}
