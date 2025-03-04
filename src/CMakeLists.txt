set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("./Checkers")
add_subdirectory("./Utils")
add_subdirectory("./LibBuilders")

add_executable(abicorn main.cpp Abicorn.cpp AbicornContext.cpp)

llvm_map_components_to_libnames(llvm_libs -17 support frontendOpenMP)

target_link_libraries(abicorn
    PRIVATE
    ${llvm_libs}
    clangAST
    clangASTMatchers
    clangBasic
    clangTooling
    clangToolingCore
    AbicornLibBuilders
    AbicornCheckers
    AbicornUtils
)