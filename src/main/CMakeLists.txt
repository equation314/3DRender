set(MODELING_SRCS modeling.cpp)

set(RENDER_SRCS render.cpp)

add_executable(modeling ${MODELING_SRCS})
target_link_libraries(modeling object)

add_executable(render ${RENDER_SRCS})
target_link_libraries(render engine)
