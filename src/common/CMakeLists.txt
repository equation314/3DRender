aux_source_directory(. COMMON_SRCS)
add_library(common ${COMMON_SRCS})
target_link_libraries(common jsoncpp_lib_static)
