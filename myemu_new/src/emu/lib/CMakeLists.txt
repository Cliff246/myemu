cmake_minimum_required(VERSION 3.10)

set(LIB_EMU ${LIB_EMU} 
    ${CMAKE_CURRENT_SOURCE_DIR}/emu.c
)

add_library(emu_lib ${LIB_EMU})


target_link_libraries(emu_lib PRIVATE myemu_utils)
target_include_directories(emu_lib PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)


