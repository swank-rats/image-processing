//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================
#include <iostream>
#include <list>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <Poco\Logger.h>
#include <Poco\AutoPtr.h>
#include <Poco\WindowsConsoleChannel.h>
#include <Poco\FormattingChannel.h>
#include <Poco\PatternFormatter.h>
#include <Poco\Util\Application.h>

#include "services\videostreaming\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"
#include "controller\VideoStreamingController.h"
#include "controller\WebSocketController.h"

#include "ThomasTestMethod.h"

using namespace cv;
using namespace std;
using Poco::Logger;
using Poco::AutoPtr;
using Poco::WindowsConsoleChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;


class ImageProcessingServerApplication : public Poco::Util::Application {
public:
	ImageProcessingServerApplication() : helpRequested(false)
	{
	}
protected:
	void initialize(Application& self) {


	}

	void reinitialize() {

	}

	void uninitialize() {

	}

	void defineOptions() {

	}

	int main(const std::vector<std::string>& args)
	{
		InitLoggers();

#ifdef THOMAS
		TestThomas();
#else
		WithThread();
#endif
		return EXIT_OK;
	}

private:
	bool helpRequested;



	void InitLoggers() {
		//setup logger
		AutoPtr<WindowsConsoleChannel> pChannel(new WindowsConsoleChannel);
		AutoPtr<PatternFormatter> pPF(new PatternFormatter);
		pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s (%p): %t");
		AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pChannel));
		Logger::root().setChannel(pFC);
	}

	void WithThread()
	{
		Logger& logger = Logger::get("main");
		logger.information("Threads were used");

		auto webcamService = std::make_shared<services::webcam::WebcamService>();

		controller::image_processing::ImageProcessingController imgProcCtrl(webcamService);
		imgProcCtrl.StartImageProcessing();

		controller::video_streaming::VideoStreamingController vidStreamCtrl(webcamService);
		vidStreamCtrl.StartStreamingServer();

		controller::websocket::WebSocketController webSocketCtrl(URI("ws://172.16.50.41:3001/"));
		webSocketCtrl.StartWebSocketClient();

		char key;
		while (1) {
			key = cvWaitKey(10);

			if (char(key) == 27){
				break; //If you hit ESC key loop will break.
			}
		}

		imgProcCtrl.StopImageProcessing();
		vidStreamCtrl.StopStreamingServer();
		webSocketCtrl.StopWebSocketClient();

		destroyAllWindows();
	}



	void TestThomas(){

		ThomasTest th;

		//th.DetectWihoutServices();
		//DetectConture();
		th.DetectConture2();
		//DetectConture3();

	}


};

POCO_APP_MAIN(ImageProcessingServerApplication);