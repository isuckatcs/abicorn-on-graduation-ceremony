enable_testing()

add_executable(unittest hash.cpp)

llvm_map_components_to_libnames(llvm_libs_unittest -17 support frontendOpenMP)

target_link_libraries(unittest
    PRIVATE
    ${llvm_libs_unittest}
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
