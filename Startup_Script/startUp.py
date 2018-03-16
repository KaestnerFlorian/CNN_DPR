from pynq import Overlay
from pynq.drivers import DMA
from pynq.drivers import HDMI
from pynq.drivers.video import VMODE_1920x1080

# Overlay
print()
print("### Overlay Configuration ###")
#ol= Overlay("convrelubuffpoolbigburst2dma.bit") # overlay for first IP core
ol = Overlay("conv1ip.bit")
ol.download() # downloading bitstream to fpga
print("IPs recognized")
print(ol.ip_dict)
print()

# HDMI
print("### HDMI Configuration ###")
hdmi_out = HDMI('out', video_mode=VMODE_1920x1080)
hdmi_in = HDMI('in')
hdmi_in.start()
hdmi_out.start()

print("HDMI IN Physical Address: ")
print(hex(hdmi_in.frame_phyaddr()))
print("HDMI OUT Physical Address: ")
print(hex(hdmi_out.frame_phyaddr()))
print()

# Memory allocation for data
print ("### Memory allocation data ###")
# 1. Input Conv1
dma0 = DMA(0x40400000,0)
inputConv1 = dma0.create_buf(0x17C4,0) # 0 means data is not cacheable
print("Physical InputConv1 Address")
print(dma0._bufPtr)

# 2. Output Conv1
dma1 = DMA(0x40400000,0)
outputConv2 = dma1.create_buf(0x19500,0)
print("Physical OutputConv2 Address")
print(dma1._bufPtr)

# 3. Weights Conv1
dma2 = DMA(0x40400000,0)
weightsConv1 = dma2.create_buf(0x500,0)
print("Physical Weights Conv1 Address")
print(dma2._bufPtr)

# 4. Bias Conv1
dma3 = DMA(0x40400000,0)
biasConv1 = dma3.create_buf(0x50,0)
print("Physical Bias Conv1 Address")
print(dma3._bufPtr)

# 5. Input Conv2
dma4 = DMA(0x40400000,0)
inputConv2 = dma4.create_buf(0x6540,0)
print("Physical InputConv2 Address")
print(dma4._bufPtr)

# 6. Output Conv2
dma5 = DMA(0x40400000,0)
outputConv2 = dma5.create_buf(0xA000,0)
print("Physical OutputConv2 Address")
print (dma5._bufPtr)

# 7. Weights Conv2
dma6 = DMA (0x40400000,0)
weightsConv2 = dma6.create_buf(0x7080,0)
print("Physcial Weights Conv2 Address")
print(dma6._bufPtr)

# 8. Bias Conv2
dma7 = DMA (0x40400000,0)
biasConv2 = dma7.create_buf(0xA0,0)
print("Physical Bias Conv2 Address")
print(dma7._bufPtr)

# 9. Input Conv3
dma8 = DMA (0x40400000,0)
inputConv3 = dma8.create_buf(0x2800,0)
print("Physical InputConv3 Address")
print(dma8._bufPtr)

# 10. Output Conv3
dma9 = DMA(0x40400000,0)
outputConv3 = dma9.create_buf(0x21C0,0)
print("Physical OutputConv3 Address")
print(dma9._bufPtr)

# 11. Weights Conv3
dma10 = DMA(0x40400000,0)
weightsConv3 = dma10.create_buf(0x15180,0)
print("Physical Weights Conv3 Address")
print(dma10._bufPtr)

# 12. Bias Conv3
dma11 = DMA (0x40400000,0)
biasConv3 = dma11.create_buf(0xF0,0)
print("Physical Address Bias Conv3")
print(dma11._bufPtr)

# 13. Input Conv4
dma12 = DMA (0x40400000,0)
inputConv4 = dma12.create_buf(0x870,0)
print("Physical Address InputConv4")
print(dma12._bufPtr)

# 14. Output Conv4
dma13 = DMA (0x40400000,0)
outputConv4 = dma13.create_buf(0x500,0)
print("Physical Address Output Conv4")
print(dma13._bufPtr)

# 15. Weights Conv4
dma14 = DMA (0x40400000,0)
weightsConv4 = dma14.create_buf(0x12C00,0)
print("Physical Address Weights Conv4")
print(dma14._bufPtr)


# 16. Bias Conv4
dma15 = DMA (0x40400000,0)
biasConv4 = dma15.create_buf(0x140,0)
print("Physical Address Bias Conv4")
print(dma15._bufPtr)


while True:
	pass
