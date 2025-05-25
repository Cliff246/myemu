
set(RISCV_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/isa_riscv.c
	
	${CMAKE_CURRENT_SOURCE_DIR}/commons/riscv_commons.c
)
set(RISCV_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/isa_riscv.h
	${CMAKE_CURRENT_SOURCE_DIR}/commons/riscv_commons.h
)

add_library(riscv_isa SHARED ${RISCV_SOURCE} ${RISCV_HEADERS})
target_link_libraries(riscv_isa PRIVATE shared_utils modules_core_interface)

target_include_directories(riscv_isa
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
