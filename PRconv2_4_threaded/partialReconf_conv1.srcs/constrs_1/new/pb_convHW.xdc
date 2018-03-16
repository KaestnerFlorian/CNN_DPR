create_pblock pb_convHW
add_cells_to_pblock [get_pblocks pb_convHW] [get_cells -quiet [list system_i/proc/convHW_depth_0]]
resize_pblock [get_pblocks pb_convHW] -add {SLICE_X26Y50:SLICE_X49Y99 SLICE_X0Y0:SLICE_X113Y49}
resize_pblock [get_pblocks pb_convHW] -add {DSP48_X2Y20:DSP48_X2Y39 DSP48_X0Y0:DSP48_X4Y19}
resize_pblock [get_pblocks pb_convHW] -add {RAMB18_X2Y20:RAMB18_X2Y39 RAMB18_X0Y0:RAMB18_X5Y19}
resize_pblock [get_pblocks pb_convHW] -add {RAMB36_X2Y10:RAMB36_X2Y19 RAMB36_X0Y0:RAMB36_X5Y9}
set_property RESET_AFTER_RECONFIG true [get_pblocks pb_convHW]
set_property SNAPPING_MODE ON [get_pblocks pb_convHW]
add_cells_to_pblock [get_pblocks pb_convHW] [get_cells -quiet [list system_i/proc/convHW_0]]

set_property HD.RECONFIGURABLE true [get_cells system_i/proc/convHW_0]

set_property HD.RECONFIGURABLE true [get_cells system_i/proc/convHW_depth_0]

