

set (DEVLIB_SOURCE  
	${CMAKE_CURRENT_SOURCE_DIR}/device_assembler_core.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_descriptor.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_emulator_core.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_files.c
	${CMAKE_CURRENT_SOURCE_DIR}/device_interface.c
	#PARENT_SCOPE
)

set (DEVLIB_HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/device_assembler_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_descriptor.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_emulator_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_files.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_interface.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_commons.h
	#PARENT_SCOPE
)

add_library(device_commons STATIC ${DEVLIB_SOURCE} ${DEVLIB_HEADERS})

target_include_directories(device_commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
