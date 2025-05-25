
cmake_minimum_required(VERSION 3.10)

add_subdirectory(myemu_core)
add_subdirectory(shared_utils)
add_subdirectory(device_libs)
add_subdirectory(modules_core)
add_subdirectory(exe)
add_subdirectory(tests)
include_directories(exe myemu_core tests device_libs modules_core)
