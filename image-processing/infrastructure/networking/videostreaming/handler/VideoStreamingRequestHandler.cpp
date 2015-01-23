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

//only for measuring 
//#include <Poco\Stopwatch.h>
//using Poco::Stopwatch;
//static double _avgenc = 0;
//static double _avgstr = 0;
//double avgenc(double newdur)
//{
//	_avgenc = 0.98*_avgenc + 0.02*newdur;
//	return _avgenc;
//}
//double avgstr(double newdur)
//{
//	_avgstr = 0.98*_avgstr + 0.02*newdur;
//	return _avgstr;
//}

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingRequestHandler::VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, NotificationQueue& lostConnectionQueue)
			: lostConnectionQueue(lostConnectionQueue), webcamService(webcamService) {
			params = { CV_IMWRITE_JPEG_QUALITY, 30 };
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
				MultipartWriter writer(out, boundary);

				Mat& image = webcamService->IsModifiedAvailable() ? webcamService->GetModifiedImage() : webcamService->GetLastImage();

				if (image.empty()) {
					logger.warning("Read empty stream image");
					continue;
				}

				//sw.restart(); //measuring encoding

				// encode mat to jpg and copy it to content
				std::vector<uchar> buf;
				cv::imencode(".jpg", image, buf, params);

				//sw.stop();
				//avgenc(sw.elapsed() * 0.001);

				MessageHeader header = MessageHeader();
				header.set("Content-Length", std::to_string(buf.size()));
				header.set("Content-Type", "image/jpeg");
				writer.nextPart(header);

				//sw.restart(); //measuring writing

				out.write(reinterpret_cast<const char*>(buf.data()), buf.size());

				//sw.stop();
				//avgstr(sw.elapsed() * 0.001);
				//printf("Encoding: %f ms; ToOut: %f ms\r", _avgenc, _avgstr);

				out << "\r\n\r\n";
			}

			logger.information("Video streaming stopped for client " + request.clientAddress().toString());
			lostConnectionQueue.enqueueNotification(new ClientConnectionLostNotification(request.clientAddress()));
		}
	}
}