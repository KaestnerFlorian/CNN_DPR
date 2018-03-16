# coding: utf-8

############################################
# caffeModel2txt prints weights and biases #
# from caffemodel to specific txt files	   #
############################################


import sys
import caffe

import os
import numpy as np
np.set_printoptions(threshold=sys.maxint) # has to be set for numpy to avoid printing (...) instead of all values

# Request for files already given as argument (recommended)
request = raw_input("Arguments allready given? (Y/N)")
if request == "Y":
    print 'Argument List:', str(sys.argv)
    net = sys.argv[1]
    model = sys.argv[2]
	
# If not already given input path to files
else:
    net = raw_input("Path to prototxt:")
    model = raw_input("Path to caffemodel:")

modelName = os.path.basename(os.path.normpath(str(model))) # extract filename from path
cnn = caffe.Net(net, model, caffe.TEST) # Read caffemodel into python

print "\nWriting network weights/ bias from *.caffemodel to txt files"
print "Producing one weights and one bias txt file for each Network layer\n"
for names, blobVec in cnn.params.iteritems():
    print "Layer Name: ", names
    shape = cnn.params[names][0].data.shape

    # Convolutional Layers for instance have a 4-dimensional shape
    if len(shape) == 4: 
	print "Amount, Depth, Height, Width" 
        print "Shape: ",shape
        width  = shape[3]
        height = shape[2]
        depth  = shape[1]
        amount = shape[0]

	# Writing Weights to txt file	
	# Description of Output txt: -all entries of a row are separated by a comma
	#			     -all rows are separated by a semicolon 
	#			     -all dimensions in depth are separated by a newline
	#			     -all different kernels are separated by "#######\n"		
        file = open("./parameters/" + str(modelName) + '_' + str(names) + '_weights.txt', "w")
        for amountCount in range (0, amount):
	    for depthCount in range(0,depth):
                for heightCount in range (0, height):
		    for widthCount in range (0, width):
                        file.write(str(cnn.params[names][0].data[amountCount][depthCount][heightCount][widthCount]))
			if widthCount == width-1:
	                    file.write(";")
			else: 
			    file.write(",")
			#print widthCount,heightCount,depthCount,amountCount
       	            if heightCount == height-1:
                        file.write("\n")
	            else:
		        pass
	        if depthCount == depth-1:
                    file.write("#\n")
	        else:
		    pass
        file.close()

	# Writing Bias to txt file
	# Description : - the bias for each neuron is separated by a newline
	file = open("./parameters/" + str(modelName) + '_' + str(names) + '_bias.txt', "w")
	biasShape = cnn.params[names][1].data.shape
	for neuron in range (0, biasShape[0]):
            file.write(str(cnn.params[names][1].data[neuron]) + "\n")
        file.close()


    # Fully connected Layers for instance have a 2-dimensional shape
    elif len(shape) == 2:
        outputs  = shape[0]
	inputs = shape[1]
        print "Filter Shape: ",shape

	# Writing Weights to txt file	
	# Description of Output txt: -each row contains the weights of all inputs for one output neuron
	file = open("./parameters/" + str(modelName) + '_' + str(names) + '_weights.txt', "w")
	for outputsCount in range (0, outputs):	    
	    for inputsCount in range (0, inputs):
		file.write(str(cnn.params[names][0].data[outputsCount][inputsCount]) + ",") 
            file.write ("\n")
	file.close()

	# Writing Bias to txt file
	# Description : - the bias for each neuron is separated by a newline
	file = open("./parameters/" + str(modelName) + '_' + str(names) + '_bias.txt', "w")
	biasShape = cnn.params[names][1].data.shape
	for neuron in range (0, biasShape[0]):
            file.write(str(cnn.params[names][1].data[neuron]) + "\n")
        file.close()

    # Not needed so not implemented yet
    elif len(shape) == 3:
	pass




