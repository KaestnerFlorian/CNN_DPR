#ifndef VIRTUAL_MEM_CALC_H
#define VIRTUAL_MEM_CALC_H

#include <iostream>
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct virtualAddress {
	void * virt_addr;
	void * mem_pointer;
};

virtualAddress virtual_mem_calc(uint32_t physical_address, uint32_t mem_size1){
int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);

	virtualAddress returnPointers;

	if(mem_dev == -1){

	   cout << "ERROR*** /dev/mem could not be opened" << endl;
	}

	// HW address provided by python HDMI Startups script
	// Describes physical memory location of stored picture
	const uint32_t mem_address_in = physical_address;

	// Size of memory for picture -> hex(1920*1080*3)
	const uint32_t mem_size = mem_size1;


	uint32_t alloc_mem_size, page_mask, page_size;
	void *mem_pointer_in, *virt_addr_in;

	page_size = sysconf(_SC_PAGESIZE);
	alloc_mem_size = (((mem_size / page_size) + 1) * page_size);
	page_mask = (page_size - 1);

	mem_pointer_in = mmap(NULL,
	                   alloc_mem_size,
	                   PROT_READ | PROT_WRITE,
	                   MAP_SHARED,
	                   mem_dev,
	                   (mem_address_in & ~page_mask)
	                   );

	if(mem_pointer_in == MAP_FAILED)
	{
	      cout << "ERROR*** mem_map failed" << endl;
	}

	virt_addr_in = (mem_pointer_in + (mem_address_in & page_mask));

	close(mem_dev);

	//cout << "Virtual Memory pointer Address: " << virt_addr_in << endl;
	returnPointers.mem_pointer =mem_pointer_in;
	returnPointers.virt_addr = virt_addr_in;

	return returnPointers;
}


#endif
