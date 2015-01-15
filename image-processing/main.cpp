//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================
#include "services\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"
#include "controller\VideoStreamingController.h"
#include "controller\WebSocketController.h"
#include "controller\ShotSimulationController.h"

#include "ThomasTestMethod.h"

#include <iostream>
#include <list>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <Poco\Logger.h>
#include <Poco\AutoPtr.h>
#include <Poco\Observer.h>
#include <Poco\WindowsConsoleChannel.h>
#include <Poco\FormattingChannel.h>
#include <Poco\PatternFormatter.h>
#include <Poco\Util\Application.h>
#include <Poco\Net\HTTPSStreamFactory.h>
#include <Poco\URI.h>
#include <Poco\Util\OptionSet.h>
#include <Poco\Util\OptionCallback.h>
#include <Poco\Util\HelpFormatter.h>
#include <Poco\SharedPtr.h>

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
using Poco::Observer;
using Poco::Net::HTTPSStreamFactory;
using Poco::Net::WebSocket;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Util::LayeredConfiguration;

using controller::image_processing::ImageProcessingController;
using controller::video_streaming::VideoStreamingController;
using controller::shot_simulation::ShotSimulationController;
using controller::websocket::WebSocketController;
using services::webcam::WebcamService;

class ImageProcessingServerApplication : public Poco::Util::Application {
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

		Application::initialize(self);

		//NOT NEEDED since it is done in config file
		////accept everything!
		//SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new ConsoleCertificateHandler(false);
		//SharedPtr<PrivateKeyPassphraseHandler> pConsoleHandler = new KeyConsoleHandler(false);

		////context = new Context(Context::CLIENT_USE, "resources\\openssl\\privkey.pem", "resources\\openssl\\cert.pem", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		//context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_STRICT, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		//context = new Poco::Net::Context(Context::CLIENT_USE, "", "rootcert.pem", "", Poco::Net::Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

		////SSLManager::instance().initializeClient(pConsoleHandler, pInvalidCertHandler, context);
		//SSLManager::instance().initializeClient(NULL, pInvalidCertHandler, context);
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

		ImageProcessingController imgProcCtrl(webcamService);
		VideoStreamingController vidStreamCtrl(webcamService);
		ShotSimulationController shotSimCtrl(webcamService, webSocketCtrl);

		webSocketCtrl->GetNotificationCenter().addObserver(Observer<ShotSimulationController, MessageNotification>(shotSimCtrl, &ShotSimulationController::HandleMessageNotification));
		webSocketCtrl->GetNotificationCenter().addObserver(Observer<VideoStreamingController, MessageNotification>(vidStreamCtrl, &VideoStreamingController::HandleMessageNotification));

		imgProcCtrl.StartImageProcessing();

		webSocketCtrl->StartWebSocketClient();

#if defined(THOMAS) || defined(STANDALONE)
		vidStreamCtrl.StartStreamingServer();
#endif

		char key;
		while (1) {
			key = cvWaitKey(10);

			if (char(key) == 27) {
				break; //If you hit ESC key loop will break.
			}
		}

		imgProcCtrl.StopImageProcessing();
		vidStreamCtrl.StopStreamingServer();
		webSocketCtrl->StopWebSocketClient();
		webSocketCtrl = nullptr; //will call destructor since it is a shared pointer

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
		th.DetectConture2();
		//DetectConture3();
		//th.Test5();
		//th.Test6();
		//th.DetectObject7();
		//th.MovingDetection();
		//th.startMotionTracking();
		//th.Test11();
#endif
	}
};

POCO_APP_MAIN(ImageProcessingServerApplication);