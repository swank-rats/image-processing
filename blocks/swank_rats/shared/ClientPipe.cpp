//============================================================================
// Name        : ClientPipe.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ClientPipe.h"

#include <stdio.h>

namespace swank_rats_lib {
	namespace pipe {
		ClientPipe::ClientPipe(const string name) {
			this->name = name;
		}

		bool ClientPipe::Connect() {
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


