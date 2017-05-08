include_directories(.)

aux_source_directory(. SRCS)
aux_source_directory(common COMMON_SRCS)
aux_source_directory(light LIGHT_SRCS)
aux_source_directory(object OBJECT_SRCS)

add_executable(main ${SRCS} ${COMMON_SRCS} ${LIGHT_SRCS} ${OBJECT_SRCS})
target_link_libraries(main jsoncpp_lib_static)
