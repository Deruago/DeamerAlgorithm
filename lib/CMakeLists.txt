file(GLOB_RECURSE HEADER_LIST "${Deamer_Algorithm_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_LIST "${Deamer_Algorithm_SOURCE_DIR}/lib/*.cpp")

add_library(Algorithm STATIC ${HEADER_LIST})
target_sources(Algorithm PRIVATE ${SOURCE_LIST})

target_include_directories(Algorithm PUBLIC 
    $<BUILD_INTERFACE:${Deamer_Algorithm_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(Algorithm PUBLIC cxx_std_17)
set_target_properties(Algorithm PROPERTIES LINKER_LANGUAGE CXX POSITION_INDEPENDENT_CODE ON)

add_library(Deamer::Algorithm ALIAS Algorithm)

# Install Targets

install(TARGETS Algorithm
    EXPORT Deamer_Algorithm_Exports
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/Deamer_Algorithm"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/Deamer_Algorithm"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/Deamer_Algorithm"
    )

export(TARGETS
	Algorithm
    NAMESPACE Deamer::
	FILE "${CMAKE_CURRENT_BINARY_DIR}/Deamer_Algorithm_Exports.cmake"
)

install(EXPORT
	Deamer_Algorithm_Exports
    NAMESPACE Deamer::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Deamer_Algorithm"
)

# Install Headers

install(DIRECTORY ${Deamer_Algorithm_SOURCE_DIR}/include/Deamer
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "LICENSE"
    )
