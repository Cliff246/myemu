cmake_minimum_required(VERSION 3.10)

set(SRC_LIB_ASM ${SRC_ASM}
	
	${CMAKE_CURRENT_SOURCE_DIR}/asm/inst.c
	${CMAKE_CURRENT_SOURCE_DIR}/asm/syntax.c
    ${CMAKE_CURRENT_SOURCE_DIR}/asm/parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/asm/asm.c
)
set(HEADER_LIB_ASM ${HEADER_LIB_ASM}
	
	${CMAKE_CURRENT_SOURCE_DIR}/asm/inst.h
    ${CMAKE_CURRENT_SOURCE_DIR}/asm/parser.h
	${CMAKE_CURRENT_SOURCE_DIR}/asm/asm.h
	${CMAKE_CURRENT_SOURCE_DIR}/asm/syntax.h
)

set(SRC_LIB_EMU
    ${CMAKE_CURRENT_SOURCE_DIR}/emu/emu.c
)

set(HEADER_LIB_EMU
    ${CMAKE_CURRENT_SOURCE_DIR}/emu/emu.h
)

add_library(myemu_lib
	${SRC_LIB_EMU} 
	${SRC_LIB_ASM}
	${HEADER_LIB_EMU}
   	${HEADER_LIB_ASM}
)


target_link_libraries(myemu_lib 
	PRIVATE
	myemu_core   
   	modules_core_runtime 
	shared_utils
)
target_include_directories(myemu_lib PRIVATE
   	${CMAKE_SOURCE_DIR}/src/myemu_core)

