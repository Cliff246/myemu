cmake_minimum_required(VERSION 3.10)

set(SRC_TESTS_ASM ${SRC_TESTS_ASM}
	${CMAKE_CURRENT_SOURCE_DIR}/asm_tests.c
)
add_executable(myemu_asm_tests ${SRC_TESTS_ASM})
target_link_libraries(myemu_asm_tests PRIVATE myemu_utils myemu_asm_lib)
target_include_directories(myemu_asm_tests PRIVATE ../utils)