cmake_minimum_required(VERSION 3.10)

add_library(myemu_core)


set(MYEMU_CORE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/cpu.c
	${CMAKE_CURRENT_SOURCE_DIR}/util_io.c
	${CMAKE_CURRENT_SOURCE_DIR}/member.c
	${CMAKE_CURRENT_SOURCE_DIR}/modules.c
)

set(MYEMU_CORE_HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
	${CMAKE_CURRENT_SOURCE_DIR}/myemu.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules.c

	${CMAKE_CURRENT_SOURCE_DIR}/util_io.h
	${CMAKE_CURRENT_SOURCE_DIR}/member.h

)


target_sources(myemu_core
	PRIVATE
	${MYEMU_CORE_SRC}
	PUBLIC
	${MYEMU_CORE_HEADERS}
)

target_include_directories(myemu_core 
	PRIVATE 
	${CJSON_INCLUDE_DIRS}
)
target_link_libraries(myemu_core 
	PRIVATE
   	${CJSON_LIBRARIES}
	dl
	PUBLIC
	modules_core_runtime
	shared_utils
)
target_include_directories(myemu_core
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

if(APPLE)
	include_directories("/opt/homebrew/include")
endif()
target_include_directories(myemu_core PRIVATE 
	${CMAKE_SOURCE_DIR}/src/myemu_core
)

