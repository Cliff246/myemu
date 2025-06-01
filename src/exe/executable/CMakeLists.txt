cmake_minimum_required(VERSION 3.10)

set(SRC_EMU
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/repl.c
)
add_executable(myemu ${SRC_EMU})
target_link_libraries(myemu PRIVATE myemu_lib shared_utils modules_core_interface)
target_include_directories(myemu PRIVATE ${CMAKE_SOURCE_DIR}/src/myemu_core ${CMAKE_SOURCE_DIR}/src/asm/lib  )
