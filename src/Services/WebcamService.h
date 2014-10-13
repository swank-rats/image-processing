/*
 * WebcamService.h
 *
 *  Created on: 11.10.2014
 *      Author: Stefan
 */

#ifndef WEBCAMSERVICE_H_
#define WEBCAMSERVICE_H_

#include <thread>

using namespace std;
using namespace cv;

class WebcamService {
public:
	WebcamService(string windowName);
	virtual ~WebcamService();

	/*
	 * This method will start the recording from the webcam
	 * Returns true if starting was successful, false otherwise
	 */
	bool startRecording();
	/*
	 * This method will stop the recodirng from the webcam
	 */
	bool stopRecording();
private:
    string windowName;
    CvCapture* capture;
    bool isStopRequested;
    thread recordingThread;

    void recording();
};

#endif /* WEBCAMSERVICE_H_ */
