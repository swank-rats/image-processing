/*
 * NetworkService.h
 *
 *  Created on: 10.10.2014
 *      Author: Stefan
 */

#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

class NetworkService {
public:
	NetworkService();
	virtual ~NetworkService();

	void StartListening(int port);
};

#endif /* NETWORKSERVICE_H_ */
