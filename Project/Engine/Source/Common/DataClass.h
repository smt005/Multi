#pragma once

#include <string>
#include <map>
#include <memory>

using namespace std;

template <class ObjectT>

class DataClassT
{
protected:
	string _name;

public:
	DataClassT() {};
	virtual ~DataClassT() {};

	virtual bool create(const string &name);

	void setName(const string& name) { _name = name; };
	const string name() { return _name; };

private:
	static map<string, shared_ptr<ObjectT>> _map;
	static shared_ptr<ObjectT> _defaultPtr;

public:
	static shared_ptr<ObjectT>& getByName(const string& name);
	static bool hasByName(const string& name);

	static void remove(const string& name);
	static void clear();
};

template <class ObjectT>
map<string, shared_ptr<ObjectT>> DataClassT<ObjectT>::_map;

template <class ObjectT>
shared_ptr<ObjectT> DataClassT<ObjectT>::_defaultPtr;

template <class ObjectT>
bool DataClassT<ObjectT>::create(const string &name)
{
	_name = name;
	return true;
};

template <class ObjectT>
shared_ptr<ObjectT>& DataClassT<ObjectT>::getByName(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
	{
		return it->second;
	}

	ObjectT* newItem = new ObjectT();
	if (newItem->create(name))
	{
		shared_ptr<ObjectT> newItemPtr(newItem);
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
bool DataClassT<ObjectT>::hasByName(const string& name)
{
	auto it = _map.find(name);
	return it != _map.end() ? true : false;
}

template <class ObjectT>
void DataClassT<ObjectT>::remove(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
	{
		_map.erase(it);
	}
}

template <class ObjectT>
void DataClassT<ObjectT>::clear()
{
	_map.clear();
}
