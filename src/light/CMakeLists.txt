aux_source_directory(. LIGHT_SRCS)
add_library(light ${LIGHT_SRCS})
target_link_libraries(light common object scene)
