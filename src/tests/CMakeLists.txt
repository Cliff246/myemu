cmake_minimum_required(VERSION 3.10)

set(TESTS_SRC ${TESTS_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

add_executable(tests ${TESTS_SRC})

target_link_libraries(tests PRIVATE myemu_core myemu_lib ${CMAKE_SOURCE_DIR}/src/exe/libs/asm)
target_include_directories(tests PRIVATE myemu_core myemu_asm_lib ../myemu_core ../asm/lib)
