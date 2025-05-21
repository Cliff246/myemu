# shared_utils/CMakeLists.txt

# List all the .c files from the sources/ folder
set(SHARED_UTILS_SRC
	
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/commons.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/constants.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/hashmap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/shared_ptr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/tokenizer.c
)

# Build the actual static library
add_library(shared_utils STATIC ${SHARED_UTILS_SRC})

# Export the header include path
target_include_directories(shared_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

