# Require (i.e. demand) an out-of-source build
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message("")
  message(STATUS "Please use an out-of-source build, it's neater.")
  message(STATUS "Don't forget to clean up ${CMAKE_BINARY_DIR} by removing:")
  message(STATUS "\tCMakeCache.txt")
  message(STATUS "\tCMakeFiles")
  message(STATUS "Then you can create a separate directory and re-run cmake from there.\n")
  message(FATAL_ERROR "In-source build attempt detected")
endif()

# JOIN and HOMEPAGE_URL arrived in 3.12
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# Put all binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project name, along with language type
project(
  inch
  HOMEPAGE_URL "https://github.com/php1ic/inch"
  LANGUAGES CXX
  VERSION 0.9.8
  )

# Add the 'cmake' folder to the list of paths to check for modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Set some global project settings
include(GeneralProjectSettings)

# Configure static analysers, this was done via scripts, but cmake can now do it
include(StaticAnalyzers)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# Which compiler flags should we use
include(CompilerWarnings)
set_project_warnings(project_warnings)

# Doxygen configuration
include(Doxygen)
enable_doxygen()

# Code Coverage Configuration
include(CodeCoverage)
enable_codecoverage(project_options)

# Allow the various santizers to be toggled ON/OFF
include(Sanitizers)
enable_sanitizers(project_options)

# Make sure any and all external libraries are linked
include(ExternalLibraries)
add_external_libraries(project_options)

# Configurable header file to store project version number
add_subdirectory(include/${PROJECT_NAME})

# Locate all of the source files apart from main.cpp
# Keep list alphabetical
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
  ${SOURCE_DIR}/converter.cpp
  ${SOURCE_DIR}/dripline.cpp
  ${SOURCE_DIR}/eps_chart.cpp
  ${SOURCE_DIR}/eps_dripline.cpp
  ${SOURCE_DIR}/eps_grid.cpp
  ${SOURCE_DIR}/eps_key.cpp
  ${SOURCE_DIR}/eps_magicNumbers.cpp
  ${SOURCE_DIR}/eps_rProcess.cpp
  ${SOURCE_DIR}/io.cpp
  ${SOURCE_DIR}/key.cpp
  ${SOURCE_DIR}/limits.cpp
  ${SOURCE_DIR}/massTable.cpp
  ${SOURCE_DIR}/nuclide.cpp
  ${SOURCE_DIR}/options.cpp
  ${SOURCE_DIR}/partition.cpp
  ${SOURCE_DIR}/rProcess.cpp
  ${SOURCE_DIR}/svg_chart.cpp
  ${SOURCE_DIR}/tikz_chart.cpp
  ${SOURCE_DIR}/ui.cpp
  )

# We are creating an object library that can be used by the test implementations
# This will stop these file being compiled twice (and referenced twice in the compilation database)
# Yes, this should probably be split out into a library, or 2, that are compiled separately and linked.
# This is referenced a few times, so future proof any changes and define as a variable
set(SOURCE_OBJECTS src_objects)
add_library(${SOURCE_OBJECTS} OBJECT ${SOURCES})

# Where are the header files
target_include_directories(${SOURCE_OBJECTS} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

# Pull in all the previous library and config we have previously done
target_link_libraries(
  ${SOURCE_OBJECTS}
  PRIVATE
  project_warnings
  project_options
  )

# We will now create the full binary, compiling main and using the object library above.
# The main.cpp file needs to be compiled with all the same options and linked with all the same libraries.
# As a results, there is a lot of repitition

# Set the executable to depend on main and the object library we have just created
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp $<TARGET_OBJECTS:${SOURCE_OBJECTS}>)

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  project_warnings
  project_options
  )

# Unit testing with Catch2
option(INCH_UNIT_TESTS "Build unit tests" OFF)
if(INCH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(external/Catch2)
  add_subdirectory(tests)
endif()

# Setup an install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
