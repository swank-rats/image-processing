#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main() {
	//  Prepare our context and socket
	void* context = zmq_init(1);
	void* socket = zmq_socket(context, ZMQ_REQ);

	std::cout << "Connecting to hello world server..." << std::endl;
	zmq_connect(socket, "tcp://localhost:5555");

	//  Do 10 requests, waiting each time for a response
	for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		zmq_send(socket, "test message", 12, 0);

		//  Get the reply.
		char* buff = new char[1024];
		zmq_recv(socket, buff, 12, 0);

		std::cout << "Received World " << request_nbr << std::endl;
	}

	zmq_close(socket);
}