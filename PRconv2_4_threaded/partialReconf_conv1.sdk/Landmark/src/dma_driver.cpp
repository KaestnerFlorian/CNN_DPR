#include "dma_driver.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>


#define DMADEBUG 0

unsigned int dma_set(unsigned int* dma_virtual_address, int offset, unsigned int value) {
    dma_virtual_address[offset>>2] = value;
}

unsigned int dma_get(unsigned int* dma_virtual_address, int offset) {
    return dma_virtual_address[offset>>2];
}

int dma_mm2s_sync(unsigned int* dma_virtual_address) {
    unsigned int mm2s_status =  dma_get(dma_virtual_address, MM2S_STATUS_REGISTER);
    while(!(mm2s_status & 1<<12) || !(mm2s_status & 1<<1) ){
        dma_s2mm_status(dma_virtual_address);
        dma_mm2s_status(dma_virtual_address);

        mm2s_status =  dma_get(dma_virtual_address, MM2S_STATUS_REGISTER);
    }
}

int dma_s2mm_sync(unsigned int* dma_virtual_address) {
    unsigned int s2mm_status = dma_get(dma_virtual_address, S2MM_STATUS_REGISTER);
    while(!(s2mm_status & 1<<12) || !(s2mm_status & 1<<1)){
        dma_s2mm_status(dma_virtual_address);
        dma_mm2s_status(dma_virtual_address);

        s2mm_status = dma_get(dma_virtual_address, S2MM_STATUS_REGISTER);
    }
}

void dma_s2mm_status(unsigned int* dma_virtual_address) {
    unsigned int status = dma_get(dma_virtual_address, S2MM_STATUS_REGISTER);
    printf("Stream to memory-mapped status (0x%08x@0x%02x):", status, S2MM_STATUS_REGISTER);
    if (status & 0x00000001) printf(" halted"); else printf(" running");
    if (status & 0x00000002) printf(" idle");
    if (status & 0x00000008) printf(" SGIncld");
    if (status & 0x00000010) printf(" DMAIntErr");
    if (status & 0x00000020) printf(" DMASlvErr");
    if (status & 0x00000040) printf(" DMADecErr");
    if (status & 0x00000100) printf(" SGIntErr");
    if (status & 0x00000200) printf(" SGSlvErr");
    if (status & 0x00000400) printf(" SGDecErr");
    if (status & 0x00001000) printf(" IOC_Irq");
    if (status & 0x00002000) printf(" Dly_Irq");
    if (status & 0x00004000) printf(" Err_Irq");
    printf("\n");
}

void dma_mm2s_status(unsigned int* dma_virtual_address) {
    unsigned int status = dma_get(dma_virtual_address, MM2S_STATUS_REGISTER);
    printf("Memory-mapped to stream status (0x%08x@0x%02x):", status, MM2S_STATUS_REGISTER);
    if (status & 0x00000001) printf(" halted"); else printf(" running");
    if (status & 0x00000002) printf(" idle");
    if (status & 0x00000008) printf(" SGIncld");
    if (status & 0x00000010) printf(" DMAIntErr");
    if (status & 0x00000020) printf(" DMASlvErr");
    if (status & 0x00000040) printf(" DMADecErr");
    if (status & 0x00000100) printf(" SGIntErr");
    if (status & 0x00000200) printf(" SGSlvErr");
    if (status & 0x00000400) printf(" SGDecErr");
    if (status & 0x00001000) printf(" IOC_Irq");
    if (status & 0x00002000) printf(" Dly_Irq");
    if (status & 0x00004000) printf(" Err_Irq");
    printf("\n");
}

void memdump(void* virtual_address, int byte_count) {
    char *p = (char*)virtual_address;
    int offset;
    for (offset = 0; offset < byte_count; offset++) {
        printf("%02x", p[offset]);
        if (offset % 4 == 3) { printf(" "); }
    }
    printf("\n");
}


void dmaSetupAndStart(	unsigned int* dma_virtual_address, unsigned int startAddress,
				unsigned int startLength, unsigned int destinationAddress,
				unsigned int destinationLength ){


	if(DMADEBUG){
		std::cout << "DMA Status before Reset" << std::endl;
		dma_mm2s_status(dma_virtual_address);
		dma_s2mm_status(dma_virtual_address);
	}

	// DMA Reset
	if(DMADEBUG){std::cout << "DMA Reset" << std::endl;}
	dma_set(dma_virtual_address, MM2S_CONTROL_REGISTER,4);
	dma_set(dma_virtual_address, S2MM_CONTROL_REGISTER,4);

	if(DMADEBUG){
		std::cout << "DMA Status after Reset" << std::endl;
		dma_mm2s_status(dma_virtual_address);
		dma_s2mm_status(dma_virtual_address);
	}

	// Halting the DMA
	if(DMADEBUG){std::cout << "DMA Halting:" << std::endl;}
    dma_set(dma_virtual_address, S2MM_CONTROL_REGISTER, 0);
    dma_set(dma_virtual_address, MM2S_CONTROL_REGISTER, 0);

	if(DMADEBUG){
		std::cout << "DMA Status after Halting" << std::endl;
		dma_mm2s_status(dma_virtual_address);
		dma_s2mm_status(dma_virtual_address);
	}

	if(DMADEBUG){std::cout << "DMA Setting Input(MM2S) and Output(S2MM) Address (32bit)" << std::endl;}
	dma_set(dma_virtual_address, MM2S_START_ADDRESS_LSB, startAddress);
    dma_set(dma_virtual_address, S2MM_DESTINATION_ADDRESS_LSB, destinationAddress);

	if(DMADEBUG){
		std::cout << "DMA Status after Setting Addresses" << std::endl;
		dma_mm2s_status(dma_virtual_address);
		dma_s2mm_status(dma_virtual_address);
	}


	// Starting DMA S2MM and MM2S
	if(DMADEBUG){std::cout << "DMA Starting SS2M and MM2S channels:" << std::endl;}
	dma_set(dma_virtual_address, S2MM_CONTROL_REGISTER, 0x1);
	dma_set(dma_virtual_address, MM2S_CONTROL_REGISTER, 0x1);

	if(DMADEBUG){
		std::cout << "DMA Status after Starting" << std::endl;
		dma_mm2s_status(dma_virtual_address);
		dma_s2mm_status(dma_virtual_address);
	}

	if(DMADEBUG){std::cout << "DMA Writing Length into S2MM and MM2S register" << std::endl;}
    dma_set(dma_virtual_address, S2MM_LENGTH, destinationLength);
    dma_set(dma_virtual_address, MM2S_LENGTH, startLength);

    if(DMADEBUG){
    		std::cout << "DMA Status after Setting Length Register" << std::endl;
    		dma_mm2s_status(dma_virtual_address);
    		dma_s2mm_status(dma_virtual_address);
    	}

}


unsigned int DmaCheck4IDLE(unsigned int* dma_virtual_address){

	if(dma_virtual_address[1] & 0x00000002){

		return 0;
	}
	else{
		//std::cout << "DMA Not IDLE" << std::endl;
		return 1;

	}

}


