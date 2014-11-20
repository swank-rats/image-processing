//============================================================================
// Name        : IObserver.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once

template<class T>
class IObserver
{
public:
	virtual void Update(T* observable) = 0;
};