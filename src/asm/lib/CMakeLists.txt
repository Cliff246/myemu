cmake_minimum_required(VERSION 3.10)

set(SRC_LIB_ASM ${SRC_ASM}
    ${CMAKE_CURRENT_SOURCE_DIR}/asm.c
)
set(HEADER_LIB_ASM ${HEADER_LIB_ASM}
	${CMAKE_CURRENT_SOURCE_DIR}/asm.h
)
add_library(myemu_asm_lib ${SRC_LIB_ASM} ${HEADER_LIB_ASM})
target_include_directories(myemu_asm_lib PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)
target_link_libraries(myemu_asm_lib PRIVATE myemu_utils)