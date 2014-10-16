/*
 * NetworkService.h
 *
 *  Created on: 10.10.2014
 *      Author: Stefan
 */

#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

class network_service {
public:
	network_service();
	virtual ~network_service();

	void start_listening(int port);
};

#endif /* NETWORKSERVICE_H_ */
