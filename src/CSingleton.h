// -----------------------------------------------------------------------------------
// Timer Fix plugin for San Andreas Multiplayer
// Copyright (c) 2018-2020, KashCherry
// -----------------------------------------------------------------------------------
#pragma once

template<typename T>
class CSingleton
{
protected:
	static T* _self;
	CSingleton() {}
	virtual ~CSingleton() {}

public:
	static T* Get()
	{
		if (!_self)
		{
			_self = new T();
		}
		return _self;
	}

	static bool Destroy()
	{
		if (_self)
		{
			delete _self;
			_self = nullptr;
			return true;
		}
		return false;
	}
};

template<typename T>
T* CSingleton<T>::_self = nullptr;