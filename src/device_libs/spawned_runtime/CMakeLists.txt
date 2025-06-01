

set(DEVICES_SPAWNED_RUNTIME_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/sources/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/sources/spawned.c
)





add_executable(device_spawned_runtime ${DEVICES_SPAWNED_RUNTIME_SOURCES})


target_link_libraries(device_spawned_runtime PRIVATE 
	shared_utils 
	modules_core_runtime
)
target_include_directories(device_spawned_runtime
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/headers
	${CMAKE_CURRENT_SOURCE_DIR}/sources
)

