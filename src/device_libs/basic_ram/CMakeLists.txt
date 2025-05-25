

set(BASIC_RAM_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/basic_ram.c
)
set(BASIC_RAM_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/basic_ram.h
)

add_library(basic_ram_device SHARED ${BASIC_RAM_SOURCE} ${BASIC_RAM_HEADERS})
target_link_libraries(basic_ram_device PRIVATE shared_utils modules_core_interface)

target_include_directories(basic_ram_device
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
