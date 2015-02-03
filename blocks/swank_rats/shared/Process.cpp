//============================================================================
// Name        : Process.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "Process.h"
#if __WINDOWS__ || __WIN32__ || _WIN32
#elif __APPLE__ && __MACH__	
//TODO hannes
#else
#error "OS not supported!"
#endif

#include <string>

using std::string;

namespace swank_rats_lib {
	namespace process {

		Process::Process(const string path) {
			this->path = path;
		}
		
		bool Process::ExecuteProcess() {
#if __WINDOWS__ || __WIN32__ || _WIN32
			STARTUPINFO si;
			char * cstr = new char[path.length() + 1];
			std::strcpy(cstr, path.c_str());

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory(&process, sizeof(process));

			// Start the child process. 
			if (!CreateProcess(cstr,	// Path to program
				NULL,					// Command line
				NULL,					// Process handle not inheritable
				NULL,					// Thread handle not inheritable
				FALSE,					// Set handle inheritance to FALSE
				NORMAL_PRIORITY_CLASS,	// No creation flags
				NULL,					// Use parent's environment block
				NULL,					// Use parent's starting directory 
				&si,					// Pointer to STARTUPINFO structure
				&process)				// Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("Create process failed (%d).\n", GetLastError());
				return false;
			}

			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		}

		bool Process::KillProcess() {
#if __WINDOWS__ || __WIN32__ || _WIN32
			if (!TerminateProcess(process.hProcess, 0)) {
				printf("Create process failed (%d).\n", GetLastError());
				return false;
			}

			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		}
	}
}


