
set(MYEMU_EXE_SRC ${MYEMU_EXE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)


add_executable(myemu ${MYEMU_EXE_SRC})
target_link_libraries(myemu PRIVATE myemu_core myemu_libs)
target_include_directories(myemu PRIVATE ${CMAKE_SOURCE_DIR}/src/myemu_core)

