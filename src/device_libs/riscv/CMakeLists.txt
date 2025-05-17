
set(RISCV_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/isa_riscv.c
	
	${CMAKE_CURRENT_SOURCE_DIR}/commons/riscv_commons.c
)
set(RISCV_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/isa_riscv.h
	${CMAKE_CURRENT_SOURCE_DIR}/commons/riscv_commons.h
)

add_library(riscv_isa SHARED ${RISCV_HEADERS} ${RISCV_HEADERS})
target_link_libraries(riscv_isa PRIVATE myemu_utils)
target_include_directories(riscv_isa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(riscv_isa
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE ${CMAKE_SOURCE_DIR}/src/utils
	PRIVATE ${CMAKE_SOURCE_DIR}/src/device_libs/commons
)
