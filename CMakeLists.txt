cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(diffraction-visualize)

# Add libs
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 11.1.4
)

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.17.0
)

CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 3.0.1
    GIT_SHALLOW
)

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.5.0
)

find_package(PkgConfig REQUIRED)
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

# Create lib target
add_library(diffraction-visualize STATIC)

target_include_directories(diffraction-visualize PUBLIC include)

target_compile_features(diffraction-visualize PUBLIC cxx_std_20)

# Link libs

target_link_libraries(diffraction-visualize PUBLIC PkgConfig::FFTW fmt::fmt SFML::Graphics CLI11::CLI11)
target_include_directories(diffraction-visualize PUBLIC PkgConfig::FFTW )


# Create binary target
add_executable(diffraction-visualize-bin)
target_link_libraries(diffraction-visualize-bin PRIVATE diffraction-visualize)

add_subdirectory(src)

# Create test target
enable_testing()

add_executable(diffraction-visualize-tests)
target_link_libraries(diffraction-visualize-tests PRIVATE diffraction-visualize GTest::gtest_main gtest)

add_subdirectory(tests)

include(GoogleTest)
gtest_discover_tests(diffraction-visualize-tests)

# Set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install needed targets
install(TARGETS diffraction-visualize
        CONFIGURATIONS Debug
        RUNTIME
          DESTINATION Debug/bin
        )

install(TARGETS diffraction-visualize
        CONFIGURATIONS Release
        RUNTIME
          DESTINATION Release/bin
        )
