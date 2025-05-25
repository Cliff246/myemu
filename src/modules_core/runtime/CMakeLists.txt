



set(MODULES_CORE_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/device_runtime/devices.c
)


add_library(modules_core_runtime STATIC ${MODULES_CORE_SRC})

target_include_directories(modules_core_runtime PUBLIC
    ${CMAKE_SOURCE_DIR}/src/modules_core/interface/device_headers
)
target_link_libraries(modules_core_runtime 
	INTERFACE modules_core_interface
	PRIVATE shared_utils
)
