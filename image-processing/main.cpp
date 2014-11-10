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
#include <Poco\Net\AcceptCertificateHandler.h>
#include <Poco\Net\InvalidCertificateHandler.h>
#include <Poco\Net\Context.h>
#include <Poco\URI.h>
#include <Poco\Util\OptionSet.h>
#include <Poco\Util\OptionCallback.h>
#include <Poco\Util\HelpFormatter.h>
#include <Poco\Net\SSLManager.h>

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
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::AcceptCertificateHandler;
using Poco::Net::Context;
using Poco::Net::SSLManager;
using Poco::Net::WebSocket;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

using controller::image_processing::ImageProcessingController;
using controller::video_streaming::VideoStreamingController;
using controller::websocket::WebSocketController;
using services::webcam::WebcamService;

class ImageProcessingServerApplication : public Poco::Util::Application {
public:
	ImageProcessingServerApplication() : helpRequested(false)
	{
	}
protected:
	void initialize(Application& self) {
		InitLoggers();

		loadConfiguration();
		Application::initialize(self);

		//accept everything!
		Poco::SharedPtr<InvalidCertificateHandler> pAcceptCertHandler = new AcceptCertificateHandler(true);
		context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		SSLManager::instance().initializeClient(NULL, pAcceptCertHandler, context);
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

	int main(const std::vector<std::string>& args)
	{
		if (!helpRequested)
		{
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
	Context::Ptr context;
	URI uri;

	void InitLoggers() {
		//setup logger
		AutoPtr<WindowsConsoleChannel> consoleChannel(new WindowsConsoleChannel);
		AutoPtr<PatternFormatter> patternFormatter(new PatternFormatter);
		patternFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s (%p): %t");
		AutoPtr<FormattingChannel> formattingChannel(new FormattingChannel(patternFormatter, consoleChannel));
		Logger::root().setChannel(formattingChannel);
	}

	void WithThread()
	{
		Logger& logger = Logger::get("main");
		logger.information("Threads were used");

		std::shared_ptr<WebcamService> webcamService = std::make_shared<WebcamService>();


		ImageProcessingController imgProcCtrl(webcamService);
		imgProcCtrl.StartImageProcessing();

		VideoStreamingController vidStreamCtrl(webcamService);
		vidStreamCtrl.StartStreamingServer();

		WebSocketController webSocketCtrl(uri, context);
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

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Websocket Performance testing client");
		helpFormatter.format(std::cout);
	}
	void HandleHelp(const std::string& name, const std::string& value)
	{
		helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void HandleURI(const std::string& name, const std::string& value)
	{
		uri = URI(value);
	}

	void TestThomas() {
		ThomasTest th;

		//th.DetectWihoutServices();
		//th.DetectConture();
		th.DetectConture2();
		//DetectConture3();
	}
};

POCO_APP_MAIN(ImageProcessingServerApplication);