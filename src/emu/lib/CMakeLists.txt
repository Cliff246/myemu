cmake_minimum_required(VERSION 3.10)

set(LIB_EMU ${LIB_EMU} 
    ${CMAKE_CURRENT_SOURCE_DIR}/emu.c
)

add_library(myemu_libs ${LIB_EMU})


target_link_libraries(myemu_libs PRIVATE myemu_core device_commons_interface)
target_include_directories(myemu_libs PRIVATE ${CMAKE_SOURCE_DIR}/src/myemu_core)


