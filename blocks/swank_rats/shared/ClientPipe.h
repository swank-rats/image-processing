//============================================================================
// Name        : ClientPipe.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once

#if __WINDOWS__ || __WIN32__ || _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>
#include <tchar.h>
#elif __APPLE__ && __MACH__	
//TODO hannes
#else
#error "OS not supported!"
#endif

#include <string>

using std::string;

namespace swank_rats_lib {
	namespace pipe {
		class ClientPipe {
		public:
			ClientPipe(const string name);
			bool Connect();
			bool Disconnect();
			bool Read(int buffSize, char *buffer);
			bool Send(const string msg);
		private:
#if __WINDOWS__ || __WIN32__ || _WIN32
			HANDLE hPipe;

#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
			string name;
		};
	}
}


