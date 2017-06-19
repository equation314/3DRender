aux_source_directory(. OBJECT_SRCS)
add_library(object ${OBJECT_SRCS})
target_link_libraries(object common util)
