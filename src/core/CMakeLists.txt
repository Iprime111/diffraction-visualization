set(DIFFRACTION_VISUALIZE_CORE_SOURCES plane_field.cpp
                                       fft.cpp
                                       transform.cpp)

list(TRANSFORM DIFFRACTION_VISUALIZE_CORE_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(diffraction-visualize PRIVATE ${DIFFRACTION_VISUALIZE_CORE_SOURCES})
