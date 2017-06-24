set(MODELING_SRCS modeling.cpp)

set(RENDER_SRCS render.cpp)

add_executable(modeling ${MODELING_SRCS})
target_link_libraries(modeling object)

add_executable(render ${RENDER_SRCS})
target_link_libraries(render engine)

add_executable(ball_pyramid scenes_gen/ball_pyramid.cpp)
target_link_libraries(ball_pyramid scene)

add_executable(bowlings scenes_gen/bowlings.cpp)
target_link_libraries(bowlings scene)

set_target_properties(ball_pyramid bowlings
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/scenes_gen"
)
