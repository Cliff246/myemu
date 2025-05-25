cmake_minimum_required(VERSION 3.10)



add_library(myemu_libs ${LIB_EMU})


target_link_libraries(myemu_libs PRIVATE myemu_core modules_core_runtime)
target_include_directories(myemu_libs PRIVATE ${CMAKE_SOURCE_DIR}/src/myemu_core)


