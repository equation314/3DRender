aux_source_directory(. SCENE_SRCS)
add_library(scene ${SCENE_SRCS})
target_link_libraries(scene object light common)
