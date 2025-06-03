
set(RISCV_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/isa_riscv.c
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/comunication_handler.c
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/device_handler.c
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/module_handler.c
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/temp.c
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/isa_riscv.c
	
	${CMAKE_CURRENT_SOURCE_DIR}/commons/sources/riscv_commons.c
	${CMAKE_CURRENT_SOURCE_DIR}/emu_core/sources/riscv_emulator.c
)

add_library(riscv_isa SHARED ${RISCV_SOURCE} ${RISCV_HEADERS})
target_link_libraries(riscv_isa PRIVATE shared_utils modules_core_interface)

target_include_directories(riscv_isa
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/commons/headers
		${CMAKE_CURRENT_SOURCE_DIR}/emu_core/headers
)
