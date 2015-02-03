//============================================================================
// Name        : ServerPipe.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ServerPipe.h"

#include <stdio.h>

namespace swank_rats_lib {
	namespace pipe {
		ServerPipe::ServerPipe(const string name) {
			this->name = name;
		}

		bool ServerPipe::Create() {
#if __WINDOWS__ || __WIN32__ || _WIN32
			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
			return true;
#else
#error "OS not supported!"
#endif
		}

	}
}


