


set (DEVLIB_HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_assembler_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_descriptor.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_emulator_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_files.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_interface.h
	${CMAKE_CURRENT_SOURCE_DIR}/headers/device_commons.h
	#PARENT_SCOPE
)



add_library(device_commons_interface INTERFACE ${DEVLIB_HEADERS})

target_include_directories(device_commons_interface INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/headers
)

