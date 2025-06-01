cmake_minimum_required(VERSION 3.10)

set(TESTS_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/shared_utils/sources/shared_utils_tests.c
	${CMAKE_CURRENT_SOURCE_DIR}/shared_utils/sources/linkedlist_tests.c
	${CMAKE_CURRENT_SOURCE_DIR}/shared_utils/sources/tokenizer_tests.c
	${CMAKE_CURRENT_SOURCE_DIR}/etc/sources/test.c
	${CMAKE_CURRENT_SOURCE_DIR}/tests_manager.c

    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)



add_executable(tests ${TESTS_SOURCE} ${TESTS_HEADERS})
target_link_libraries(tests PRIVATE myemu_core myemu_lib)
target_include_directories(tests PRIVATE myemu_core ../myemu_core)

target_include_directories(tests PRIVATE

	${CMAKE_CURRENT_SOURCE_DIR}/etc/headers
	${CMAKE_CURRENT_SOURCE_DIR}/shared_utils/headers
	${CMAKE_CURRENT_SOURCE_DIR}
)
