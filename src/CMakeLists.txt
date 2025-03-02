set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("./Checkers")
add_subdirectory("./Utils")
add_subdirectory("./LibBuilders")

add_executable(abicorn main.cpp Abicorn.cpp AbicornContext.cpp)

llvm_map_components_to_libnames(llvm_libs support frontendOpenMP)

target_link_libraries(abicorn
    ${llvm_libs}
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangSerialization
    clangTooling
    clangRewrite
    AbicornLibBuilders
    AbicornCheckers
    AbicornUtils
)