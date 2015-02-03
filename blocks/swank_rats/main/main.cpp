//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================
#include "services\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\WebCamController.h"
#include "controller\VideoStreamingController.h"
#include "controller\WebSocketController.h"
#include "controller\ShotSimulationController.h"
#include "shared\model\message\MessageHeaders.h"

#include "ThomasTestMethod.h"

#include <iostream>
#include <list>

#include "diego/zmqcpp/zmq.hpp"

#include "diego/opencv/opencv2/core/core.hpp"
#include "diego/opencv/opencv2/highgui/highgui.hpp"

#include "fenix/poco/Foundation/include/Poco/Foundation.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"
#include "fenix/poco/Foundation/include/Poco/WindowsConsoleChannel.h"
#include "fenix/poco/Foundation/include/Poco/FormattingChannel.h"
#include "fenix/poco/Foundation/include/Poco/PatternFormatter.h"
#include "fenix/poco/Util/include/Poco/Util/Application.h"
#include "fenix/poco/Util/include/Poco/Util/ServerApplication.h"
#include "fenix/poco/NetSSL_OpenSSL/include/Poco/Net/HTTPSStreamFactory.h"
#include "fenix/poco/Foundation/include/Poco/URI.h"
#include "fenix/poco/Util/include/Poco/Util/OptionSet.h"
#include "fenix/poco/Util/include/Poco/Util/OptionCallback.h"
#include "fenix/poco/Util/include/Poco/Util/HelpFormatter.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/Thread.h"

using namespace cv;
using namespace std;
using Poco::Logger;
using Poco::SharedPtr;
using Poco::AutoPtr;
using Poco::URI;
using Poco::WindowsConsoleChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Observer;
using Poco::Net::HTTPSStreamFactory;
using Poco::Net::WebSocket;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Util::LayeredConfiguration;
using Poco::Util::ServerApplication;

using shared::model::message::MessageCommandEnum;
using controller::webcam::WebCamController;
using controller::video_streaming::VideoStreamingController;
using controller::shot_simulation::ShotSimulationController;
using controller::websocket::WebSocketController;
using services::webcam::WebcamService;

class ImageProcessingServerApplication : public ServerApplication {
public:
	ImageProcessingServerApplication() : helpRequested(false)
	{
		Poco::Net::initializeSSL();
		Poco::Net::HTTPSStreamFactory::registerFactory();
	}

	~ImageProcessingServerApplication() {
		Poco::Net::uninitializeSSL();
	}
protected:
	void initialize(Application& self) {
		InitLoggers();
		loadConfiguration();

		ServerApplication::initialize(self);
	}

	void reinitialize(Application& self) {
		Application::reinitialize(self);
	}

	void uninitialize() {
		Application::uninitialize();
	}

	void defineOptions(OptionSet& options) {
		Application::defineOptions(options);
		options.addOption(
			Option("help", "h", "display help information")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<ImageProcessingServerApplication>(this, &ImageProcessingServerApplication::HandleHelp)));

		options.addOption(
			Option("uri", "u", "Address to node js (websockt-)server e.g. ws://127.0.0.1:3001/")
			.required(true)
			.repeatable(false)
			.argument("addr")
			.callback(OptionCallback<ImageProcessingServerApplication>(this, &ImageProcessingServerApplication::HandleURI)));
	}

	int main(const std::vector<std::string>& args) {
		if (!helpRequested)	{
#ifdef THOMAS
			TestThomas();
#else
			WithThread();
#endif
		}

		return EXIT_OK;
	}

private:
	bool helpRequested;
	URI uri;

	void InitLoggers() {
		//setup logger
		AutoPtr<WindowsConsoleChannel> consoleChannel(new WindowsConsoleChannel);
		AutoPtr<PatternFormatter> patternFormatter(new PatternFormatter);
		patternFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s (%p): %t");
		AutoPtr<FormattingChannel> formattingChannel(new FormattingChannel(patternFormatter, consoleChannel));
		Logger::root().setChannel(formattingChannel);
	}

	void WithThread() {
		Logger& logger = Logger::get("main");
		logger.information("Threads were used");

		SharedPtr<WebcamService> webcamService(new WebcamService());
		SharedPtr<WebSocketController> webSocketCtrl(new WebSocketController(uri));

		WebCamController webCamCtrl(webcamService);
		VideoStreamingController vidStreamCtrl(webcamService, webSocketCtrl);
		ShotSimulationController shotSimCtrl(webcamService, webSocketCtrl);

		webSocketCtrl->GetNotificationCenter().addObserver(Observer<ShotSimulationController, MessageNotification>(shotSimCtrl, &ShotSimulationController::HandleMessageNotification));
		webSocketCtrl->GetNotificationCenter().addObserver(Observer<VideoStreamingController, MessageNotification>(vidStreamCtrl, &VideoStreamingController::HandleMessageNotification));

		webCamCtrl.StartWebCam();
		shotSimCtrl.StartSimulationService();
		webSocketCtrl->StartWebSocketClient();

#if defined(CONNTEST)

		Thread::sleep(3500);
		webSocketCtrl->Send(new Message(MessageCommandEnum::hit));
		logger.information("CLOSE SERVER NOW");

		Thread::sleep(2000);
		webSocketCtrl->Send(new Message(MessageCommandEnum::hit));
		Thread::sleep(500);
		webSocketCtrl->Send(new Message(MessageCommandEnum::hit));
		Thread::sleep(500);
		webSocketCtrl->Send(new Message(MessageCommandEnum::hit));
#endif

#if defined(THOMAS) || defined(STANDALONE)
		vidStreamCtrl.StartStreamingServer();

		
		
		//shotSimCtrl.StartTestingSimulation();
#endif

		char key;
		while (1) {
			key = cvWaitKey(10);

			if (char(key) == 27) {
				break; //If you hit ESC key loop will break.
			}
		}
		shotSimCtrl.StopSimulationService();
		webCamCtrl.StopWebCam();
		vidStreamCtrl.StopStreamingServer();
		webSocketCtrl->StopWebSocketClient();
		webSocketCtrl = nullptr;

		destroyAllWindows();
	}

	void displayHelp() {
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Websocket Performance testing client");
		helpFormatter.format(std::cout);
	}
	void HandleHelp(const std::string& name, const std::string& value) {
		helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void HandleURI(const std::string& name, const std::string& value) {
		uri = URI(value);
	}

	void TestThomas() {
#ifdef THOMAS
		ThomasTest th;

		//th.DetectWihoutServices();
		//th.DetectConture();
		//th.DetectConture2();
		//th.DetectCircle();
		th.DetectCircle4();
		//th.DetectAll();
		//DetectConture3();
		//th.Test5();
		//th.Test6();
		//th.DetectObject7();
		//th.MovingDetection();
		//th.startMotionTracking();
		//th.Test11();
		//th.WallDetectionTestOne();
#endif
	}
};

POCO_SERVER_MAIN(ImageProcessingServerApplication);
