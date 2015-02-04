#include "diego/opencv/opencv2/objdetect/objdetect.hpp"
#include "diego/opencv/opencv2/highgui/highgui.hpp"
#include "diego/opencv/opencv2/imgproc/imgproc.hpp"

#include "swank_rats\shared\ClientPipe.h"
#include "swank_rats\shared\ServerPipe.h"
#include "swank_rats\shared\Process.h"

#include "diego/zmqcpp/zmq.hpp"

#include <string>
#include <iostream>

using swank_rats_lib::process::Process;

int main()
{
	Process *p = new Process("MJPEGPerformanceTest.exe");

	p->ExecuteProcess();

	Sleep(1000);

	p->KillProcess();

	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);

	std::cout << "Connecting to hello world server..." << std::endl;
	socket.connect("tcp://localhost:5555");

	//  Do 10 requests, waiting each time for a response
	for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		zmq::message_t request(6);
		memcpy((void *)request.data(), "Hello", 5);
		std::cout << "Sending Hello " << request_nbr << "..." << std::endl;
		socket.send(request);

		//  Get the reply.
		zmq::message_t reply;
		socket.recv(&reply);
		std::cout << "Received World " << request_nbr << std::endl;
	}
	return 0;
}