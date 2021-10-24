cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(Deamer_Algorithm VERSION 1.1
                  DESCRIPTION "Deamer CC algorithms."
                  LANGUAGES CXX)

set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" FORCE)
endif()

# version information
if(NOT DEFINED Deamer_Algorithm_VERSION_MAJOR)
  set(Deamer_Algorithm_VERSION_MAJOR 1)
endif()
if(NOT DEFINED Deamer_Algorithm_VERSION_MINOR)
  set(Deamer_Algorithm_VERSION_MINOR 0)
endif()
if(NOT DEFINED Deamer_Algorithm_VERSION_PATCH)
  set(Deamer_Algorithm_VERSION_PATCH 0)
endif()

add_subdirectory(lib)
add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -V)

find_package(Doxygen)
if(Doxygen_FOUND)
	add_subdirectory(docs)
else()
	message(STATUS "Doxygen not found, not building docs")
endif()

option(PACKAGE_TESTS "Build the tests" OFF)
if(PACKAGE_TESTS)
	find_package(GTest)
	if (GTEST_FOUND)
		enable_testing()
		add_subdirectory(tests)
	else()
		enable_testing()
		include(GoogleTest)
		add_subdirectory(tests)
	endif()
endif()

install(DIRECTORY ${Deamer_Algorithm_SOURCE_DIR}/include/Deamer
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "LICENSE"
    )

export(TARGETS
	Deamer_Algorithm
	FILE "${CMAKE_CURRENT_BINARY_DIR}/Deamer_AlgorithmConfig.cmake"
)

install(EXPORT
	Deamer_AlgorithmConfig
	DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Deamer_Algorithm"
)