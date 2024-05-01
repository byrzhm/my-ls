cmake_minimum_required(VERSION 3.20.0)

project(list LANGUAGES C)

# Sanitizer
if (NOT DEFINED SANITIZER)
    set(SANITIZER address)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Werror -Wno-sign-compare -fsanitize=${SANITIZER} -fno-omit-frame-pointer -O0 -g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(app)
