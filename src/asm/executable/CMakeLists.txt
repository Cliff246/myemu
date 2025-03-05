cmake_minimum_required(VERSION 3.10)

set(SRC_ASM ${SRC_ASM}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)
add_executable(myemu_asm ${SRC_ASM})
target_link_libraries(myemu_asm PRIVATE myemu_utils myemu_asm_lib)
target_include_directories(myemu_asm PRIVATE ../utils ../lib)