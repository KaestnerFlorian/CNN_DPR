# CNN Dynamic Partial Reconfiguration

## Bitstreams 
Contains full and partial bitstreams for different convolution IP cores. It also contains tcl script to be usable with PYNQ framework

## PRconv*
Contains application sources for DPR with conv1/3 and conv2/4 created with XSDK
Besides specific Landmark.elf is available.

## Startup_Script
Startup Script allocates continuous memory and programs FPGA. It also initializes the HDMI in and output used by the application.


## caffe2txtparser
Generates txt files with weights and bias values extracted from prototxt and caffemodel files. 
Prototxt and caffemodel files provided by: https://github.com/luoyetx/deep-landmark 


## conv_layer
Sources for HLS core development of convolution layers using Xilinx HLS.


## Prerequisites
- openCV 2.x
- Pynq Framework tag v1.4

## Howto
The application searches for weight extracted to txt format in "home/resources/weights" directory on Pynq. 
Bitstreams must be located "home/pynq/bitstreams"
1. Connect Pynq to HDMI source and sink
2. Execute startUp.py to program FPGA, allocate continous memory and initialize HDMI (sudo)
3. As root start Landmark.elf file which should be located in home



## Citation
Please cite if it helps your research:

		@INPROCEEDINGS {floriankästnerbenediktjanßenfrederikkautzmichaelhübnergiuliocorradi2018,
		  author    = "Florian Kästner, Benedikt Janßen, Frederik Kautz, Michael Hübner and Giulio Corradi",
		  title     = "Hardware/Software Codesign for Convolutional Neural Networks exploiting Dynamic Partial Reconfiguration on PYNQ",
		  booktitle = "32nd Annual IEEE International Parallel \& Distributed Processing Symposium (IEEE IPDPS 2018)",
		  year      = "2018"
	    }
