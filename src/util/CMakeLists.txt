aux_source_directory(. UTIL_SRCS)
add_library(util ${UTIL_SRCS})
target_link_libraries(util common)
