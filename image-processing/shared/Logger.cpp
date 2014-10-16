//============================================================================
// Name        : Logger.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#include <iostream>
#include "Logger.h"

using namespace std;

namespace Logger{
	void Logger::addMessage(string msg) {
		cout << msg << endl;
	}

	void Logger::addError(string msg) {
		cerr << msg << endl;
	}
}