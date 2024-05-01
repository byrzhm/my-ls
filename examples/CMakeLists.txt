file(GLOB EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*/*.c")
message("EXAMPLES: ${EXAMPLES}")

foreach(EXAMPLE ${EXAMPLES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE listutil)
    message("EXAMPLE_NAME: ${EXAMPLE_NAME}")
endforeach(EXAMPLE)