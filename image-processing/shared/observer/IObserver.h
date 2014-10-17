//============================================================================
// Name        : IObserver.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#pragma once

template<class T>
class IObserver
{
public:
	virtual void Update(T* observable) = 0;
};