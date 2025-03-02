set(LIT_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/lit_runner.py")

if(DEFINED LLVM_BUILD_DIR)
    set(LIT_EXECUTABLE ${LLVM_DEFAULT_EXTERNAL_LIT})
endif()

message(STATUS "Using lit: ${LIT_EXECUTABLE}")

# Perform testing with llvm-lit.
add_custom_target(test ${LIT_EXECUTABLE} -v -s --path=${CMAKE_BINARY_DIR} --path=${LLVM_TOOLS_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR} DEPENDS abicorn USES_TERMINAL)
