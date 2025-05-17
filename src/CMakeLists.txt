
cmake_minimum_required(VERSION 3.10)


add_subdirectory(emu)
add_subdirectory(asm)
add_subdirectory(tests)
add_subdirectory(utils)
add_subdirectory(device_libs)
include_directories(asm emu utils tests)
