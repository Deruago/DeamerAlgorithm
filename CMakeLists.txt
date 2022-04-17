cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(Deamer_Algorithm VERSION 1.1.0
                  DESCRIPTION "Deamer CC algorithms."
                  LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" FORCE)
endif()

add_subdirectory(lib)

macro(option_enable OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
	if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
		option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
	else()
		option(${OPTION_NAME} ${OPTION_TEXT} OFF)
	endif()
endmacro()

option_enable(DEAMER_ALGORITHM_ENABLE_DOXYGEN "Build the examples" OFF)
option_enable(DEAMER_ALGORITHM_BUILD_EXAMPLES "Build the examples" OFF)
option_enable(DEAMER_ALGORITHM_BUILD_TESTS "Build the tests" ON)

if(DEAMER_ALGORITHM_ENABLE_DOXYGEN)
	find_package(Doxygen)
	if(Doxygen_FOUND)
		add_subdirectory(docs)
	else()
		message(STATUS "Doxygen not found, not building docs")
	endif()
endif()

if(DEAMER_ALGORITHM_BUILD_EXAMPLES)
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

# Package

include(CMakePackageConfigHelpers)

configure_package_config_file(${Deamer_Algorithm_SOURCE_DIR}/cmake/Deamer_AlgorithmConfig.cmake.in
  "${Deamer_Algorithm_BINARY_DIR}/Deamer_AlgorithmConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Deamer_Algorithm
)

write_basic_package_version_file(
	"Deamer_AlgorithmConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(FILES
	${Deamer_Algorithm_BINARY_DIR}/Deamer_AlgorithmConfig.cmake
	${Deamer_Algorithm_BINARY_DIR}/Deamer_AlgorithmConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Deamer_Algorithm
)