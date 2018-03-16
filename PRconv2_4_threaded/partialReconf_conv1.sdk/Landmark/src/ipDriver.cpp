/*
 * ipDriver.cpp
 *
 *  Created on: 04.09.2017
 *      Author: frederik
 */
#include "ipDriver.h"
#include <iostream>


void IPSetupAndStart(unsigned int* ip_virtual_address, unsigned int WeightsAddress, unsigned int BiasAddress){

	// Setting Address Registers
	ip_virtual_address[4] = WeightsAddress;
	ip_virtual_address[6] = BiasAddress;

	//Starting IP
	ip_virtual_address[0]= 0x1;



}


unsigned int IPRunCheck(unsigned int* ip_virtual_address){


    if (1== ip_virtual_address[0]){
    	return 0;
    }
    else {
    	return 1;
    }

}


unsigned int IPWait4Finish (unsigned int* ip_virtual_address){

	int timeOutCount = 0;
	bool breaker = false;
	while(!(ip_virtual_address[0] == 0x4 || ip_virtual_address[0] == 0x6)){

		timeOutCount++;
		if(timeOutCount>100000){
			breaker = true;
			break;
		}
	}

	if (true == breaker){
		std::cout << "IP Core TimeOut" << std::endl;
		return 1;}
	else{
		return 0;
	}
}
