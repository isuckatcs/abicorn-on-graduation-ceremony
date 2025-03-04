cmake_minimum_required(VERSION 3.13.4)
project(abicorn VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(COVERAGE_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fno-exceptions")
SET(CMAKE_CXX_FLAGS_COVERAGE "${COVERAGE_FLAGS}")
SET(CMAKE_C_FLAGS_COVERAGE "${COVERAGE_FLAGS}")

find_package(LLVM 17.0.0 REQUIRED)
find_package(Clang 17.0.0 REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message(STATUS "Found Clang ${Clang_PACKAGE_VERSION}")
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

set(Clang_INSTALL_DIR ${CLANG_INSTALL_PREFIX})

if(NOT Clang_INSTALL_DIR)
    set(Clang_INSTALL_DIR ${LLVM_BINARY_DIR})
endif()

message(STATUS "Using LLVM/Clang from: ${Clang_INSTALL_DIR}")

# version dir can appear in multiple forms (e.g.: 14.0.6 or 16), so do the lookup based on the major version
file(GLOB CLANG_STDLIB_PATH "${Clang_INSTALL_DIR}/lib/clang/${LLVM_VERSION_MAJOR}*/include")
list(LENGTH CLANG_STDLIB_PATH PATH_LENGTH)

if("${PATH_LENGTH}" GREATER "1")
    message(FATAL_ERROR "More than 1 resource path to clang found with version ${LLVM_VERSION_MAJOR}.\n${CLANG_STDLIB_PATH}")
endif()

# pass it down to the app to configure the compiler accordingly
message(STATUS "Using Clang stdlib: ${CLANG_STDLIB_PATH}")
add_definitions(-DCLANG_STDLIB_PATH="${CLANG_STDLIB_PATH}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories("./include")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c- /GR-")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
endif()

add_subdirectory(src)
add_subdirectory(test)

if(BUILD_UNITTESTS)
    set(GTest_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third-party/googletest")

    message(STATUS "Configuring googletest")

    file(MAKE_DIRECTORY ${GTest_DIR}/build)
    execute_process(COMMAND ${CMAKE_COMMAND} -S . -B ./build -G ${CMAKE_GENERATOR} -DBUILD_GMOCK=OFF WORKING_DIRECTORY ${GTest_DIR} OUTPUT_QUIET)
    execute_process(COMMAND ${CMAKE_COMMAND} --build ./build WORKING_DIRECTORY ${GTest_DIR} OUTPUT_QUIET)

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(${GTest_DIR})
    add_subdirectory(unittest)

    add_custom_target(unit ctest --test-dir ${CMAKE_CURRENT_BINARY_DIR}/unittest DEPENDS unittest USES_TERMINAL)
endif()
