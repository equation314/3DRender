set(MODELING_SRCS modeling.cpp)

set(RAY_TRACING_SRCS raytracing.cpp)

add_executable(modeling ${MODELING_SRCS})
target_link_libraries(modeling object)

add_executable(ray_tracing ${RAY_TRACING_SRCS})
target_link_libraries(ray_tracing engine)
