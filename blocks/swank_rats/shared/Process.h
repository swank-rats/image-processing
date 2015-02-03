//============================================================================
// Name        : Process.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once

#if __WINDOWS__ || __WIN32__ || _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <tchar.h>
#elif __APPLE__ && __MACH__	
//TODO hannes
#else
#error "OS not supported!"
#endif

#include <string>

using std::string;

namespace swank_rats_lib {
	namespace process {
		class Process {
		public:
			Process(const string path);
			bool ExecuteProcess();
			bool KillProcess();
		private:
			string path;
#if __WINDOWS__ || __WIN32__ || _WIN32
			PROCESS_INFORMATION process;
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		};
	}
}

