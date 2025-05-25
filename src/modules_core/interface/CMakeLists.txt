set (MODULES_CORE_INTERFACE_DEVICE_HEADERS 
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_assembler_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_descriptor.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_emulator_core.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_files.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_interface.h
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers/device_commons.h
	#PARENT_SCOPE
)

set (MODULES_CORE_INTERFACE_MODULE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/modules_headers/modules_commons.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules_headers/modules_description.h
	${CMAKE_CURRENT_SOURCE_DIR}/modules_headers/modules_top
)

add_library(modules_core_interface INTERFACE 
	${MODULES_CORE_INTERFACE_DEVICE_HEADERS}
	${MODULES_CORE_INTERFACE_MODULE_HEADERS}
)

target_include_directories(modules_core_interface INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/device_headers
	${CMAKE_CURRENT_SOURCE_DIR}/modules_headers
)
