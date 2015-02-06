//============================================================================
// Name        : VideoStreamingRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "VideoStreamingRequestHandler.h"
#include "..\..\..\..\shared\notifications\ClientConnectionLostNotification.h"

#include <Poco\Net\MultipartWriter.h>
#include <Poco\Net\MessageHeader.h>

using Poco::Net::MessageHeader;
using Poco::Net::HTTPResponse;
using Poco::Net::MultipartWriter;
using shared::notifications::ClientConnectionLostNotification;

namespace infrastructure {
	namespace video_streaming {
		// only for measurement
		//bool _qpcInited = false;
		//double PCFreq = 0.0;
		//__int64 CounterStart = 0;
		//void InitCounter()
		//{
		//	LARGE_INTEGER li;
		//	if (!QueryPerformanceFrequency(&li))
		//	{
		//		std::cout << "QueryPerformanceFrequency failed!\n";
		//	}
		//	PCFreq = double(li.QuadPart) / 1000.0f;
		//	_qpcInited = true;
		//}
		//double CLOCK()
		//{
		//	if (!_qpcInited) InitCounter();
		//	LARGE_INTEGER li;
		//	QueryPerformanceCounter(&li);
		//	return double(li.QuadPart) / PCFreq;
		//}

		//double _avgdur = 0;
		//double _fpsstart = 0;
		//double _avgfps = 0;
		//double _fps1sec = 0;

		//double avgdur(double newdur)
		//{
		//	_avgdur = 0.98*_avgdur + 0.02*newdur;
		//	return _avgdur;
		//}

		VideoStreamingRequestHandler::VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, VideoStreamingServer& server)
			: webcamService(webcamService), server(server) {
			boundary = "VIDEOSTREAM";
		}

		VideoStreamingRequestHandler::~VideoStreamingRequestHandler() {
			//do not delete, since it is a shared pointer
			webcamService = nullptr;
		}

		void VideoStreamingRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("VideoStreamingRequestHandler");
			logger.information("Video stream request by client: " + request.clientAddress().toString());

			// check if webcam service is running correctly
			if (!webcamService->IsRecording()) {
				logger.warning("No stream available. Closing connection to " + request.clientAddress().toString());
				response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
				response.send();
				return;
			}

			logger.information("Video streaming started for client " + request.clientAddress().toString());

			response.set("Max-Age", "0");
			response.set("Expires", "0");
			response.set("Cache-Control", "no-cache, private");
			response.set("Pragma", "no-cache");
			response.setContentType("multipart/x-mixed-replace; boundary=--" + boundary);
			response.setChunkedTransferEncoding(false);

			std::ostream& out = response.send();
			int frames = 0;
			//double start = 0.0;
			//double dif = 0.0;

			while (out.good() && webcamService->IsRecording()) {
				//start = CLOCK();

				MultipartWriter writer(out, boundary);

				vector<uchar>* buf = webcamService->GetModifiedImage(); //take ownership

				if (buf->size() == 0) {
					logger.warning("Read empty stream image");
					delete buf;
					continue;
				}

				MessageHeader header = MessageHeader();
				header.set("Content-Length", std::to_string(buf->size()));
				header.set("Content-Type", "image/jpeg");
				writer.nextPart(header);
				out.write(reinterpret_cast<const char*>(buf->data()), buf->size());
				out << "\r\n\r\n";

				delete buf;
				buf = nullptr;

				//dif = CLOCK() - start;
				//printf("Sending: %.2f ms; avg: %.2f ms\r", dif, avgdur(dif));
				++frames;
			}

			printf("Frames: %u", frames);

			logger.information("Video streaming stopped for client " + request.clientAddress().toString());
			server.HandleClientLostConnection(request.clientAddress());
		}
	}
}