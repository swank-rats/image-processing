//============================================================================
// Name        : Thread.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#include <windows.h>

#include "Logger.h"
#include "Thread.h"

Thread::Thread()
{
	isRunning = false;
	isStopRequested = false;
	threadId = 0;
	joinTime = 100;
	stopTries = 5;
}

Thread::~Thread()
{
	Stop();
}

void Thread::Start() {
	LPVOID param = this;
	threadHandle = CreateThread(NULL, 0, StaticRun, (void*) this, 0, &threadId);
}

bool Thread::Stop() {
	isStopRequested = true;
	int tries = 0;

	do {
		Sleep(joinTime);
		++tries;
	} while (isRunning && stopTries >= tries);

	if (tries >= stopTries) {
		DWORD exitCode = 0;
		BOOL success = TerminateThread(threadHandle, exitCode);
		
		if (!success) {
			Logger::addError("Stopping thread failed:" + exitCode);

			return false;
		}
	}

	return true;
}

DWORD Thread::RunTilStopRequested() {
	isRunning = true; 
	
	do {
		run();
	} while (!isStopRequested);

	isRunning = false;

	return 0;
}

DWORD WINAPI Thread::StaticRun(LPVOID param) {
	Thread* thread = reinterpret_cast<Thread*>(param);
	if (!thread) {
		Logger::addError("Thread starting failed");
		return 1;
	}

	return thread->RunTilStopRequested();
}