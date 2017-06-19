set(MODELING_SRCS modeling.cpp)

set(RAY_TRACING_SRCS raytracing.cpp)

set(PHOTON_MAPPING_SRCS photonmapping.cpp)

add_executable(modeling ${MODELING_SRCS})
target_link_libraries(modeling object)

add_executable(ray_tracing ${RAY_TRACING_SRCS})
target_link_libraries(ray_tracing engine)

add_executable(photon_mapping ${PHOTON_MAPPING_SRCS})
target_link_libraries(photon_mapping engine)
