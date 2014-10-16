/*
 * WebcamService.h
 *
 *  Created on: 11.10.2014
 *      Author: Stefan
 */

#ifndef WEBCAMSERVICE_H_
#define WEBCAMSERVICE_H_

//#include <boost/thread.hpp>
#include <thread>

using namespace std;
using namespace cv;

class webcam_service {
public:
	webcam_service(string windowName);
	virtual ~webcam_service();

	/*
	 * This method will start the recording from the webcam
	 * Returns true if starting was successful, false otherwise
	 */
	bool start_recording();
	/*
	 * This method will stop the recodirng from the webcam
	 */
	bool stop_recording();
private:
    string windowName;
    CvCapture* capture;
    bool isStopRequested;
    thread recordingThread;

    void recording();
};

#endif /* WEBCAMSERVICE_H_ */
