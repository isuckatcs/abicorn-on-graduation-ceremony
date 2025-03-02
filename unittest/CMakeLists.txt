enable_testing()

add_executable(unittest hash.cpp)

llvm_map_components_to_libnames(llvm_libs support frontendOpenMP)

target_link_libraries(unittest
    ${llvm_libs}
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangSerialization
    clangTooling
    AbicornUtils
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unittest)
