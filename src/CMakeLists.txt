
cmake_minimum_required(VERSION 3.10)

add_subdirectory(emu)
add_subdirectory(asm)
add_subdirectory(tests)
add_subdirectory(myemu_core)
add_subdirectory(shared_utils)
add_subdirectory(device_libs)
add_subdirectory(device_interface)
include_directories(asm emu myemu_core tests device_libs device_interface)
