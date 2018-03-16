/*
 * dma_driver.h
 *
 *  Created on: 18.08.2017
 *      Author: frederik
 */

#ifndef SRC_DMA_DRIVER_H_
#define SRC_DMA_DRIVER_H_

#define MM2S_CONTROL_REGISTER 0x0
#define MM2S_STATUS_REGISTER 0x04
#define MM2S_START_ADDRESS_LSB 0x18
#define MM2S_START_ADDRESS_MSB 0x1C
#define MM2S_LENGTH 0x28

#define S2MM_CONTROL_REGISTER 0x30
#define S2MM_STATUS_REGISTER 0x34
#define S2MM_DESTINATION_ADDRESS_LSB 0x48
#define S2MM_DESTINATION_ADDRESS_MSB 0x4c
#define S2MM_LENGTH 0x58


unsigned int dma_set(unsigned int* dma_virtual_address, int offset, unsigned int value);


unsigned int dma_get(unsigned int* dma_virtual_address, int offset);


int dma_s2mm_sync(unsigned int* dma_virtual_address);

int dma_mm2s_sync(unsigned int* dma_virtual_address);

void dma_s2mm_status(unsigned int* dma_virtual_address);


void dma_mm2s_status(unsigned int* dma_virtual_address);


void memdump(void* virtual_address, int byte_count);

void dmaSetupAndStart(	unsigned int* dma_virtual_address, unsigned int startAddress,
				unsigned int startLength, unsigned int destinationAddress,
				unsigned int destinationLength );

unsigned int DmaCheck4IDLE(unsigned int* dma_virtual_address);

#endif /* SRC_DMA_DRIVER_H_ */
