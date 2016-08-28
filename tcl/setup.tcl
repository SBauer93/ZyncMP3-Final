###############################################################################
#
# setup.tcl - Setup a Vivado HLS project.
#
# Author:   Jens Rudolf (Uni Rostock)
# Date:     2016-07-04
#
# Changelog:
#       2016-07-04 - Initial version
#
# Copyright (c) 2015 by Jens Rudolf (Uni Rostock). All rights
# reserved. For educational use only. NO WARRANTY!
# 
# Report bugs to:
#       jens.rudolf@uni-rostock.de
# 
###############################################################################

set PROJECT_NAME hls
set PROJECT_ROOT [pwd]/..

set SOURCES_PATH ${PROJECT_ROOT}/src
set INCLUDE_PATH ${PROJECT_ROOT}/include

set HW_INPUT_FIFO  [pwd]/DATA_IN
set HW_OUTPUT_FIFO [pwd]/DATA_OUT

# Open vivado project
open_project -reset ${PROJECT_NAME}

# Set the top level module for synthesis
set_top Filter

# Add the design source files
add_files ${SOURCES_PATH}/Filter.cpp -cflags "-I${INCLUDE_PATH}"

# Add the simulation source files
add_files -tb ${SOURCES_PATH}/Consumer.cpp  -cflags "-I${INCLUDE_PATH}"
add_files -tb ${SOURCES_PATH}/Producer.cpp  -cflags "-I${INCLUDE_PATH}"
add_files -tb ${SOURCES_PATH}/Testbench.cpp -cflags "-I${INCLUDE_PATH}"

# Open the default solution
open_solution -reset "solution1"

# Set target hardware and clock
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

# Close project and exit
close_project
exit
