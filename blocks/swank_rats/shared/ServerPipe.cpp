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
#if __WINDOWS__ || __WIN32__ || _WIN32
			hPipe = INVALID_HANDLE_VALUE;
#elif __APPLE__ && __MACH__	
			//TODO hannes
#else
#error "OS not supported!"
#endif
		}

		bool ServerPipe::Read(int buffSize, char *buffer) {
#if __WINDOWS__ || __WIN32__ || _WIN32
			LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

			DWORD cbBytesRead = 0, cbWritten = 0;
			BOOL fSuccess = FALSE;
			HANDLE hPipe = CreateNamedPipe(
				name.c_str(),				// pipe name 
				PIPE_ACCESS_DUPLEX,			// read/write access 
				PIPE_TYPE_MESSAGE |			// message type pipe 
				PIPE_READMODE_MESSAGE |		// message-read mode 
				PIPE_WAIT,					// blocking mode 
				PIPE_UNLIMITED_INSTANCES,	// max. instances  
				buffSize,					// output buffer size 
				buffSize,					// input buffer size 
				0,							// client time-out 
				NULL);						// default security attribute 

			if (hPipe == INVALID_HANDLE_VALUE)
			{
				printf("CreateNamedPipe failed, GLE=%d.\n", GetLastError());
				return false;
			}

			// Read client requests from the pipe. This simplistic code only allows messages
			// up to BUFSIZE characters in length.
			fSuccess = ReadFile(
				hPipe,			// handle to pipe 
				buffer,			// buffer to receive data 
				sizeof(TCHAR),	// size of buffer 
				&cbBytesRead,	// number of bytes read 
				NULL);			// not overlapped I/O 

			if (!fSuccess || cbBytesRead == 0)
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{
					printf("Client disconnected.\n", GetLastError());
				}
				else
				{
					printf("ReadFile failed, GLE=%d.\n", GetLastError());
				}

				return false;
			}

			FlushFileBuffers(hPipe);
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
			return true;
#elif __APPLE__ && __MACH__	
			//TODO hannes
			return true;
#else
#error "OS not supported!"
#endif
		}

		bool ServerPipe::Send(int buffSize, string msg) {
#if __WINDOWS__ || __WIN32__ || _WIN32
			LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 

			DWORD cbBytesRead = 0, cbWritten = 0;
			BOOL fSuccess = FALSE;
			HANDLE hPipe = CreateNamedPipe(
				name.c_str(),				// pipe name 
				PIPE_ACCESS_DUPLEX,			// read/write access 
				PIPE_TYPE_MESSAGE |			// message type pipe 
				PIPE_READMODE_MESSAGE |		// message-read mode 
				PIPE_WAIT,					// blocking mode 
				PIPE_UNLIMITED_INSTANCES,	// max. instances  
				buffSize,					// output buffer size 
				buffSize,					// input buffer size 
				0,							// client time-out 
				NULL);						// default security attribute 

			if (hPipe == INVALID_HANDLE_VALUE)
			{
				printf("CreateNamedPipe failed, GLE=%d.\n", GetLastError());
				return false;
			}

			// Write the reply to the pipe. 
			fSuccess = WriteFile(
				hPipe,			// handle to pipe 
				msg.c_str(),    // buffer to write from 
				cbBytesRead,	// number of bytes to write 
				&cbWritten,		// number of bytes written 
				NULL);			// not overlapped I/O 

			if (!fSuccess || cbBytesRead != cbWritten)
			{
				printf("WriteFile failed, GLE=%d.\n", GetLastError());
				return false;
			}
				
			FlushFileBuffers(hPipe);
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
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


