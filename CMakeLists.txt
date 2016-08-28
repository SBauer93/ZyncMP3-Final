# ============================================================================
#
# Copyright 2009-2016 University of Rostock
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#
# ============================================================================
cmake_minimum_required(VERSION 2.8)


project("ZynqMP3")


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(SystemC REQUIRED 2.3)
find_package(Vivado REQUIRED 2014.4 COMPONENT HLS)


# We need to include the SystemC headers
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories(${SystemC_INCLUDE_DIRS})


# The names and location of the unix pipes to be created
set(HW_INPUT_FIFO "${CMAKE_BINARY_DIR}/HW_FIFO_IN")
set(HW_OUTPUT_FIFO "${CMAKE_BINARY_DIR}/HW_FIFO_OUT")

# This is required for GCC >= 6.1
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This is required to allow the type casts in the minimp3
add_definitions("-fpermissive")


# ------------------------ SystemC Model -------------------------------------
#
# This is the simulation (model) executable. Change this according to your
# source file names and paths.
#
add_executable(model
    "${CMAKE_SOURCE_DIR}/src/Filter.cpp"
    "${CMAKE_SOURCE_DIR}/src/Consumer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Producer.cpp"
    "${CMAKE_SOURCE_DIR}/src/Testbench.cpp"
)
# ----------------------------------------------------------------------------



# --------------------------- MP3 Player -------------------------------------
#
# This is the player executable. Change this according to your source file
# names and paths.
#
add_executable(player
    "${CMAKE_SOURCE_DIR}/src/minimp3.cpp"
    "${CMAKE_SOURCE_DIR}/src/player_alsa.cpp"
)
# ----------------------------------------------------------------------------


# Link the player against libasound
target_link_libraries(player asound)


# Link the testbench against the SystemC library.
target_link_libraries(model ${SystemC_LIBRARIES})



# Create FIFOs
add_custom_command(
    OUTPUT ${HW_INPUT_FIFO} ${HW_OUTPUT_FIFO}
    COMMAND "echo" " * Creating UNIX pipe ${HW_INPUT_FIFO}"
    COMMAND "mkfifo" ${HW_INPUT_FIFO}
    COMMAND "echo" " * Creating UNIX pipe ${HW_OUTPUT_FIFO}"
    COMMAND "mkfifo" ${HW_OUTPUT_FIFO}
)


# Start the SystemC simulation
add_custom_target(sim
    COMMAND "echo" " * Starting testbench in background"
    COMMAND "model" "&"
    COMMAND "echo" " * Starting player in foreground"
    COMMAND "player" "${CMAKE_SOURCE_DIR}/Suerte.mp3"
    DEPENDS model player ${HW_INPUT_FIFO} ${HW_OUTPUT_FIFO}
)


# Setup HLS project
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/hls/vivado_hls.app"
    COMMAND "echo" " * Creating HLS project"
    COMMAND
        "${Vivado_HLS_EXECUTABLE}"
        "-f" "${CMAKE_SOURCE_DIR}/tcl/setup.tcl"
)


# Start the High-Level Synthesis
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/hls/solution1/syn"
    COMMAND "echo" " * Starting High-Level Synthesis"
    COMMAND
        "${Vivado_HLS_EXECUTABLE}"
        "-f" "${CMAKE_SOURCE_DIR}/tcl/csynth.tcl"
    DEPENDS "${CMAKE_BINARY_DIR}/hls/vivado_hls.app"
)


# Start the RTL Co-Simulation
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/hls/solution1/sim"
    COMMAND "echo" " * Starting RTL Co-Simulation"
    COMMAND
        "${Vivado_HLS_EXECUTABLE}"
        "-f" "${CMAKE_SOURCE_DIR}/tcl/cosim.tcl"
    DEPENDS
        "${CMAKE_BINARY_DIR}/hls/solution1/syn"
        player
        ${HW_INPUT_FIFO} ${HW_OUTPUT_FIFO}
)


# Start the RTL Co-Simulation
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/hls/solution1/impl"
    COMMAND "echo" " * Exporting IP package"
    COMMAND
        "${Vivado_HLS_EXECUTABLE}"
        "-f" "${CMAKE_SOURCE_DIR}/tcl/export.tcl"
    DEPENDS
        "${CMAKE_BINARY_DIR}/hls/solution1/syn"
)



add_custom_target(hls
    DEPENDS "${CMAKE_BINARY_DIR}/hls/solution1/syn"
)

add_custom_target(cosim
    DEPENDS "${CMAKE_BINARY_DIR}/hls/solution1/sim"
)

add_custom_target(export
    DEPENDS "${CMAKE_BINARY_DIR}/hls/solution1/impl"
)



# Setup Vivado RTL project
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/rtl/rtl.xpr"
    COMMAND "echo" " * Creating Vivado project"
    COMMAND "${Vivado_EXECUTABLE}" "-nolog" "-nojournal" "-mode" "batch" "-source"
            "${CMAKE_SOURCE_DIR}/xillybus/vhdl/xillydemo-vivado.tcl"
    DEPENDS "${CMAKE_BINARY_DIR}/hls/solution1/impl"
)

add_custom_target(rtl
    DEPENDS "${CMAKE_BINARY_DIR}/rtl/rtl.xpr"
)
