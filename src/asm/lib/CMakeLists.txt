cmake_minimum_required(VERSION 3.10)
#file is /sr/asm/lib/cmakelists.txt
set(SRC_LIB_ASM ${SRC_ASM}
	
	${CMAKE_CURRENT_SOURCE_DIR}/inst.c
	${CMAKE_CURRENT_SOURCE_DIR}/syntax.c
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/asm.c
)
set(HEADER_LIB_ASM ${HEADER_LIB_ASM}
	
	${CMAKE_CURRENT_SOURCE_DIR}/inst.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
	${CMAKE_CURRENT_SOURCE_DIR}/asm.h
	${CMAKE_CURRENT_SOURCE_DIR}/syntax.h
)

add_library(myemu_asm_lib ${SRC_LIB_ASM} ${HEADER_LIB_ASM})
find_package(cJSON REQUIRED)
message(STATUS ${CJSON_INCLUDE_DIRS})
message(STATUS ${CJSON_LIBRARIES})


if(APPLE)
	include_directories("/opt/homebrew/include")
endif()
target_include_directories(myemu_asm_lib 
	PRIVATE 
		${CMAKE_SOURCE_DIR}/src/myemu_core

	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}

)
target_link_libraries(myemu_asm_lib PRIVATE myemu_core)


