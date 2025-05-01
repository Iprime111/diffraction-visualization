set(TEST_SOURCES plane_field_tests.cpp)
list(TRANSFORM TEST_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(diffraction-visualize-tests PRIVATE ${TEST_SOURCES})
