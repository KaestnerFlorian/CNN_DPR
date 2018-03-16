/*
 * ipDriver.h
 *
 *  Created on: 04.09.2017
 *      Author: frederik
 */

#ifndef SRC_IPDRIVER_H_
#define SRC_IPDRIVER_H_


void IPSetupAndStart(unsigned int* ip_virtual_address, unsigned int WeightsAddress, unsigned int BiasAddress);

unsigned int IPRunCheck(unsigned int* ip_virtual_address);

unsigned int IPWait4Finish(unsigned int* ip_virtual_address);

#endif /* SRC_IPDRIVER_H_ */
