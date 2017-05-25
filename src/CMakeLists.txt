include_directories(.)

aux_source_directory(common COMMON_SRCS)
aux_source_directory(curve CURVE_SRCS)
aux_source_directory(light LIGHT_SRCS)
aux_source_directory(object OBJECT_SRCS)
aux_source_directory(scene SCENE_SRCS)
aux_source_directory(raytracing RAY_TRACING_SRCS)
aux_source_directory(modeling MODELING)

add_executable(raytracing ${COMMON_SRCS} ${CURVE_SRCS} ${LIGHT_SRCS} ${OBJECT_SRCS} ${SCENE_SRCS} ${RAY_TRACING_SRCS})
add_executable(modeling ${COMMON_SRCS} ${CURVE_SRCS} ${OBJECT_SRCS} ${MODELING})
target_link_libraries(raytracing jsoncpp_lib_static)
target_link_libraries(modeling jsoncpp_lib_static)
