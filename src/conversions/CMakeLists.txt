set(DIFFRACTION_VISUALIZE_CONVERSIONS_SOURCES wavelength_lookup.cpp)

list(TRANSFORM DIFFRACTION_VISUALIZE_CONVERSIONS_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(diffraction-visualize PRIVATE ${DIFFRACTION_VISUALIZE_CONVERSIONS_SOURCES})
