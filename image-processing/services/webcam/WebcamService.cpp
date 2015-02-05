//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebcamService.h"
#include <iostream>
#include <iomanip>

#include <gst\gst.h>
#include <gst\gstelement.h>
#include <gst\gstpipeline.h>
#include <gst\gstutils.h>
#include <gst\app\gstappsrc.h>

#include <Poco\Clock.h>

#include <Poco\Stopwatch.h>
using Poco::Stopwatch;

using Poco::Clock;
using Poco::Thread;
using std::cout;

namespace services {
	namespace webcam {
		WebcamService::WebcamService() : capture(VideoCapture()), recordingActivity(this, &WebcamService::RecordingCore) {
			isRecording = false;
			params = { CV_IMWRITE_JPEG_QUALITY, 30 };
			fps = 15;
			delay = 1000 / fps;
		}

		WebcamService::~WebcamService() {
			if (recordingActivity.isRunning()) {
				StopRecording();
			}

			if (capture.isOpened()) {
				capture.release();
			}
		}

		int WebcamService::GetDelay() {
			return delay;
		}

		int WebcamService::GetFPS() {
			return fps;
		}

		void WebcamService::SetModifiedImage(Mat& image) {
			Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
			// encode mat to jpg and copy it to content
			cv::imencode(".jpg", image, modifiedImage, params);
		}

		vector<uchar>* WebcamService::GetModifiedImage() {
			Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
			vector<uchar> *tempImg = new vector<uchar>(modifiedImage.begin(), modifiedImage.end());
			return tempImg;
		}

		Mat& WebcamService::GetLastImage() {
			Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
			return lastImage;
		}

		bool WebcamService::StartRecording() {
			Logger& logger = Logger::get("WebcamService");

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				logger.error("No camera available!");
				return false;
			}

			logger.information("starting recording...");

			//camera settings
			capture.set(CV_CAP_PROP_FPS, fps);
			//Possible resolutions : 1280x720, 640x480; 440x330
			capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

			logger.information("Camera settings: ");
			logger.information("FPS: " + std::to_string(capture.get(CV_CAP_PROP_FPS)));
			logger.information("Resolution: " + std::to_string(capture.get(CV_CAP_PROP_FRAME_WIDTH)) + "x" + std::to_string(capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
			logger.information("Codec: " + std::to_string(capture.get(CV_CAP_PROP_FOURCC)));
			logger.information("Format: " + std::to_string(capture.get(CV_CAP_PROP_FORMAT)));

			recordingActivity.start();

			logger.information("started recording");

			return true;
		}

		bool WebcamService::StopRecording() {
			Logger& logger = Logger::get("WebcamService");

			logger.information("stopping recording...");

			if (recordingActivity.isRunning()) {
				recordingActivity.stop();
				logger.information("recording activity stop requested");
				recordingActivity.wait();
				logger.information("recording activity stopped successfully");
			}
			else {
				logger.error("recording activity is already stopped!");
			}

			logger.information("stopped recording");

			return true;
		}

		bool WebcamService::IsRecording() {
			return capture.isOpened() && recordingActivity.isRunning();
		}

		void WebcamService::RecordingCore() {
			Logger& logger = Logger::get("WebcamService");
			GstElement *pipeline;
			GstElement *source, *filter, *sink;
			guint64 imagecounter;

			/* create pipeline */
			pipeline = gst_pipeline_new("my-pipeline");

			/* create elements */
			source = gst_element_factory_make("fakesrc", "source");
			filter = gst_element_factory_make("identity", "filter");
			sink = gst_element_factory_make("fakesink", "sink");

			/* must add elements to pipeline before linking them */
			gst_bin_add_many(GST_BIN(pipeline), source, filter, sink, NULL);

			/* link */
			if (!gst_element_link_many(source, filter, sink, NULL)) {
				g_warning("Failed to link elements!");
			}

			Mat frame;

			//Stopwatch sw;
			Clock clock;
			int newDelay = 0;

			while (!recordingActivity.isStopped()) {
				if (!capture.isOpened()) {
					logger.error("Lost connection to webcam!");
					break;
				}

				//sw.restart();

				//Create image frames from capture
				capture >> frame;

				clock.update();
				if (!frame.empty()) {
						{
							Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
							lastImage = frame; //Clone image
						}

					//sw.stop();
					//printf("Capture frame: %f ms\n", sw.elapsed() * 0.001);
					//sw.restart();

					//Notify();

					/// OpenCV handles image in BGR, so to get RGB, Channels R and B needs to be swapped.
					//cv::cvtColor(image, image, CV_CVTIMG_SWAP_RB);

					{
						/// How do i get the actual bpp and depth out of the cv::Mat?
						GstCaps *caps = gst_caps_new_simple("video/x-raw-rgb", "width", G_TYPE_INT, frame.cols, "height", G_TYPE_INT,
							frame.rows, "framerate", GST_TYPE_FRACTION, 0, 1, NULL);

						g_object_set(G_OBJECT(source), "caps", caps, NULL);
						gst_caps_unref(caps);

						IplImage* img = new IplImage(frame);

						GstBuffer *buffer;
						GstMapInfo map;
						map.data = (uchar*)img->imageData;

						/// Copy Data from OpenCV to GStreamer
						if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
							gst_util_dump_mem(map.data, map.size);
							gst_buffer_unmap(buffer, &map);
						}
						//{
						//	int bufferlength = frame.cols * frame.rows * frame.channels();
						//	buffer = gst_buffer_new_and_alloc(bufferlength);

						//	memcpy(GST_BUFFER_DATA(buffer), IMG_data, GST_BUFFER_SIZE(buffer));

						//	GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale(bufferlength, GST_SECOND, 1);
						//}

						/// Setting the Metadata for the image to be pushed.
						{
							GstCaps *caps_Source = NULL;

							std::stringstream video_caps_text;
							video_caps_text << "video/x-raw-rgb,width=(int)" << frame.cols << ",height=(int)" << frame.rows << ",framerate=(fraction)0/1";
							caps_Source = gst_caps_from_string(video_caps_text.str().c_str());

							if (!GST_IS_CAPS(caps_Source)) {
								std::cout << "Error creating Caps for OpenCV-Source, exiting...";
								exit(1);
							}

							gst_app_src_set_caps(GST_APP_SRC(source), caps_Source);
							//gst_buffer_set_caps(buffer, caps_Source);
							//gst_app_src_push_buffer(caps_Source->, buffer);
							gst_caps_unref(caps_Source);
						}

						/// Setting a continues timestamp
						GST_BUFFER_TIMESTAMP(buffer) = gst_util_uint64_scale(imagecounter * 20, GST_MSECOND, 1);
						imagecounter += 1;

						/// Push Buffer into GStreamer-Pipeline
						GstFlowReturn rw;
						rw = gst_app_src_push_buffer(GST_APP_SRC(source), buffer);

						if (rw != GST_FLOW_OK) {
							std::cout << "Error push buffer to GStreamer-Pipeline, exiting...";

							exit(1);
						}
						else {
							std::cout << "GST_FLOW_OK " << "imagecounter: " << imagecounter << std::endl;
						}
					}

					//sw.stop();
					//printf("Notifiy: %f ms\n", sw.elapsed() * 0.001);
				}
				else {
					logger.warning("Captured empty webcam frame!");
				}

				newDelay = delay - clock.elapsed() * 0.001;

				if (newDelay > 0) {
					//webcam can only be queried after some time again
					//according to the FPS rate
					Thread::sleep(newDelay);
				}
			}
		}
	}
}