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

find_package(PkgConfig REQUIRED)
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)

# Create binary
add_executable(diffraction-visualize)

target_include_directories(diffraction-visualize PRIVATE include)

target_compile_features(diffraction-visualize PRIVATE cxx_std_20)

# Link fftw

target_link_libraries(diffraction-visualize PRIVATE PkgConfig::FFTW)
target_include_directories(diffraction-visualize PRIVATE PkgConfig::FFTW)

add_subdirectory(src)

# TODO test target

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
