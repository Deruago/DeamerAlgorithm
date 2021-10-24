file(GLOB_RECURSE HEADER_LIST "${Deamer_Algorithm_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_LIST "${Deamer_Algorithm_SOURCE_DIR}/lib/*.cpp")

add_library(Deamer_Algorithm STATIC ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(Deamer_Algorithm PUBLIC 
    $<BUILD_INTERFACE:${Deamer_Algorithm_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/>
)

target_compile_features(Deamer_Algorithm PUBLIC cxx_std_17)
set_target_properties(Deamer_Algorithm PROPERTIES LINKER_LANGUAGE CXX POSITION_INDEPENDENT_CODE ON)

add_library(Deamer_Algorithm_Shared SHARED ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(Deamer_Algorithm_Shared PUBLIC 
    $<BUILD_INTERFACE:${Deamer_Algorithm_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/>
)

target_compile_features(Deamer_Algorithm_Shared PUBLIC cxx_std_17)
set_target_properties(Deamer_Algorithm_Shared PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS Deamer_Algorithm
    EXPORT Deamer_AlgorithmConfig
    DESTINATION lib/Deamer_Algorithm
    )