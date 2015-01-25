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

//#include <Poco\Stopwatch.h>
//using Poco::Stopwatch;

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingRequestHandler::VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, NotificationQueue& lostConnectionQueue)
			: lostConnectionQueue(lostConnectionQueue), webcamService(webcamService) {
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

			//Stopwatch sw;
			
			while (out.good() && webcamService->IsRecording()) {
				//sw.restart(); //measuring writing

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

				//sw.stop();
				//printf("Sending: %f ms\n\r", sw.elapsed() * 0.001);
			}

			logger.information("Video streaming stopped for client " + request.clientAddress().toString());
			lostConnectionQueue.enqueueNotification(new ClientConnectionLostNotification(request.clientAddress()));
		}
	}
}