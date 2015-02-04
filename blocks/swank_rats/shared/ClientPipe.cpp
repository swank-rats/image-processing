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

#if __WINDOWS__ || __WIN32__ || _WIN32
			hPipe = INVALID_HANDLE_VALUE;
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		}

		bool ClientPipe::Connect() {
#if __WINDOWS__ || __WIN32__ || _WIN32
			char * cstr = new char[name.length() + 1];
			std::strcpy(cstr, name.c_str());

			hPipe = CreateFile(
				cstr,			// pipe name 
				GENERIC_READ |  // read and write access 
				GENERIC_WRITE,
				0,              // no sharing 
				NULL,           // default security attributes
				OPEN_EXISTING,  // opens existing pipe 
				0,              // default attributes 
				NULL);          // no template file 

			// return if the pipe handle is valid. 
			if (hPipe != INVALID_HANDLE_VALUE) {
				return false;
			}

			// Exit if an error other than ERROR_PIPE_BUSY occurs. 
			if (GetLastError() != ERROR_PIPE_BUSY)
			{
				printf("Could not open pipe. GLE=%d.\n", GetLastError());
				return false;
			}

			// All pipe instances are busy, so wait for 20 seconds. 
			if (!WaitNamedPipe(cstr, 20000))
			{
				printf("Could not open pipe: 20 second wait timed out.");
				return false;
			}

			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
			return true;
#else
#error "OS not supported!"
#endif
		}

		bool ClientPipe::Read(int buffSize, char *buffer) {
#if __WINDOWS__ || __WIN32__ || _WIN32
			// The pipe connected; change to message-read mode. 
			DWORD dwMode = PIPE_READMODE_MESSAGE;
			bool fSuccess = SetNamedPipeHandleState(
				hPipe,    // pipe handle 
				&dwMode,  // new pipe mode 
				NULL,     // don't set maximum bytes 
				NULL);    // don't set maximum time 

			if (!fSuccess)
			{
				printf("SetNamedPipeHandleState failed. GLE=%d.\n", GetLastError());
				return false;
			}
			DWORD cbRead;
			fSuccess = false;

			// Read from the pipe. 
			fSuccess = ReadFile(
				hPipe,					// pipe handle 
				buffer,					// buffer to receive reply 
				buffSize*sizeof(TCHAR),	// size of buffer 
				&cbRead,				// number of bytes read 
				NULL);					// not overlapped 

			if (!fSuccess) {
				printf("Read failed. GLE=%d.\n", GetLastError());
				return false;
			}

			printf("Read from pipe: \"%s\"\n", buffer);

			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
			return true;
#else
#error "OS not supported!"
#endif
		}

		bool ClientPipe::Send(const string msg) {
#if __WINDOWS__ || __WIN32__ || _WIN32
			// The pipe connected; change to message-read mode. 
			DWORD dwMode = PIPE_READMODE_MESSAGE;
			bool fSuccess = SetNamedPipeHandleState(
				hPipe,    // pipe handle 
				&dwMode,  // new pipe mode 
				NULL,     // don't set maximum bytes 
				NULL);    // don't set maximum time 

			if (!fSuccess)
			{
				printf("SetNamedPipeHandleState failed. GLE=%d.\n", GetLastError());
				return false;
			}

			// Send a message to the pipe server. 
			DWORD cbWritten;
			DWORD cbToWrite = (lstrlen(msg.c_str()) + 1)*sizeof(TCHAR);
			printf("Sending %d byte message: \"%s\"\n", cbToWrite, msg);

			fSuccess = WriteFile(
				hPipe,			// pipe handle 
				msg.c_str(),	// message 
				cbToWrite,		// message length 
				&cbWritten,		// bytes written 
				NULL);			// not overlapped 

			if (!fSuccess)
			{
				printf("WriteFile to pipe failed. GLE=%d\n", GetLastError());
				return false;
			}

			printf("\nMessage sent %d bytes to server\n", cbWritten);
			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
			return true;
#else
#error "OS not supported!"
#endif
		}

		bool ClientPipe::Disconnect() {
#if __WINDOWS__ || __WIN32__ || _WIN32
			return CloseHandle(hPipe);
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		}

	}
}


