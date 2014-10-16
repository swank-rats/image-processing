//============================================================================
// Name        : Thread.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#pragma once

#include <windows.h>
class Thread
{
	public:
		Thread();
		~Thread();
	protected:
		// This method must contain the code that should be executed
		virtual void run() = 0;
		// This will start the thread and calls the run method
		void Start();
		// This will stop the thread (it will wait until it has finished its work)
		bool Stop();
	private:
		// State of the thread
		bool isRunning;
		// State if thread will be stopped
		bool isStopRequested;
		// Time waiting til thread is stopped (in ms)
		int joinTime;
		// Tries until thread is killed hardcore
		int stopTries;
		// ID of the thread
		DWORD threadId;
		// Handle of the Thread
		HANDLE threadHandle;
		// This method calls the run() method in a loop
		DWORD RunTilStopRequested();
		// This method is passed to CreateThread
		static DWORD WINAPI StaticRun(LPVOID param);
};

