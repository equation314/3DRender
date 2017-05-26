include_directories(.)

aux_source_directory(common COMMON_SRCS)
add_library(common ${COMMON_SRCS})

aux_source_directory(curve CURVE_SRCS)
add_library(curve ${CURVE_SRCS})

aux_source_directory(light LIGHT_SRCS)
add_library(light ${LIGHT_SRCS})

aux_source_directory(object OBJECT_SRCS)
add_library(object ${OBJECT_SRCS})
target_link_libraries(object curve common)

aux_source_directory(scene SCENE_SRCS)
add_library(scene ${SCENE_SRCS})
target_link_libraries(scene object light common)

aux_source_directory(raytracing RAY_TRACING_SRCS)
add_executable(raytracing ${RAY_TRACING_SRCS})
target_link_libraries(raytracing scene jsoncpp_lib_static)

aux_source_directory(modeling MODELING)
add_executable(modeling ${MODELING})
target_link_libraries(modeling object jsoncpp_lib_static)
