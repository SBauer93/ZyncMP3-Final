###############################################################################
#
# csynth.tcl - Run Vivado High-Level Synthesis.
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

# Open vivado project and default solution
open_project ${PROJECT_NAME}
open_solution "solution1"

# Run Higl-Level Synthesis
csynth_design

# close project and exit Vivado
close_project
exit
