#include "diego/opencv/opencv2/objdetect/objdetect.hpp"
#include "diego/opencv/opencv2/highgui/highgui.hpp"
#include "diego/opencv/opencv2/imgproc/imgproc.hpp"

#include "swank_rats\shared\ClientPipe.h"

#include "diego/zmqcpp/zmq.hpp"

#include <string>
#include <iostream>

using swank_rats_lib::pipe::ClientPipe;

int main()
{
	ClientPipe clientPipe("\\\\.\\pipe\\swankratspipe");

	zmq::context_t context(1); //number of threads used for IO
	zmq::socket_t socket(context, ZMQ_REQ);
	socket.connect("tcp://localhost:5555");

	zmq::message_t init(4);
	memcpy((void *)init.data(), "INIT", 4);
	printf("Sending INIT to server\n");
	socket.send(init);

	while (1) {
		zmq::message_t reply;
		socket.recv(&reply); //is blocking? 

		printf("Received: %s", reply.data());

		if (reply.data() == "END") {
			break;
		}
		else if (reply.data() == "OPENPIPE") {
			clientPipe.Connect();
		}
		else if (reply.data() == "IMAGE") {
			//clientPipe.Read()
		}
		else {
			zmq::message_t response(6);
			memcpy((void *)response.data(), "INVALID", 6);
			printf("Invalid message received. Response with INVALID\n");
			socket.send(response);
		}

		
	}
	return 0;
}