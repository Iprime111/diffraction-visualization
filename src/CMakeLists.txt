set(DIFFRACTION_VISUALIZE_SOURCES main.cpp)
list(TRANSFORM DIFFRACTION_VISUALIZE_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(diffraction-visualize-bin PRIVATE ${DIFFRACTION_VISUALIZE_SOURCES})

add_subdirectory(core)
add_subdirectory(conversions)
add_subdirectory(physics)
