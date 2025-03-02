![cover](img/definitely_not_ai_generated_cover.png)

It's exactly like those graduation gift balloons, except this is a Clang Tool that detects library-level API and ABI compatibility breaking changes based on source code.

## 🦄 Get Started 🦄

This guide assumes that the project is being built on Linux* but equivalent steps can be performed on any other operating system.

1. Clone the third-party dependencies
    ```
    git submodule init && git submodule update
    ```

2. Install CMake 3.13.4 or newer
    ```
    apt-get install cmake
    ```

3. Create a build directory for LLVM and enter that directory
    ```
    mkdir third-party/llvm/build && cd third-party/llvm/build
    ```

4. Configure and build the Clang Tools dependency
    ```
    cmake ../llvm                                                \
      -DLLVM_ENABLE_PROJECTS="clang"           \
      -DLLVM_TARGETS_TO_BUILD="host"                             \
      -DCMAKE_BUILD_TYPE=MinSizeRel &&                           \
    cmake --build .
    ```

5. Create a build directory and enter that directory
    ```
    mkdir build && cd build
    ```

6. Configure and build the project
   ```
   cmake ../ -DLLVM_BUILD_DIR="../third-party/llvm/build" && cmake --build .
   ```

To run the tests, proceed with these following optional steps.

7. Install Python 3.10 or newer
    ```
    apt-get install python
    ```

8. Install the required dependencies
    ```
    pip install lit
    ```

9. Run the `unit` target with CMake
    ```
    cmake --build . --target unit
    ```

10. Run the `test` target with CMake
    ```
    cmake --build . --target test
    ```

\* tested on Ubuntu 22.04.3 LTS

## 🎓 Running the Tool 🎓

To compare the old and the new version of the same library, just give abicorn the list of source files that make up the library.
```
$ abicorn --old source1.cpp source2.cpp ... --new source1.cpp source2.cpp ... --
```

If both versions of the library has to be compiled with specific flags, pass the flags after the `--` in the end.

```
$ abicorn --old source.cpp --new source.cpp -- -std=c++11
```

Flags can also be applied individually to each specific version with the `--s-old` and `--s-new` options. 

```
$ abicorn --old source.cpp -s-old -I/path/to/old/include                       \
          --new source.cpp -s-new -I/path/to/new/include --
```

If there is a `compile_commands.json` available for a library, abicorn will deduce the flags that should be passed to `clang` from it, if the build directory of the library is specified with the `--p-old` and `--p-new` options.

```
$ abicorn --old source.cpp --p-old=/path/to/dir/of/compile_commands.json       \
          --new source.cpp --p-new=/path/to/dir/of/compile_commands.json --
```

## 🎈 Detected Problems 🎈

The tool doesn't rely on any compiler specific implementation details, only on the wording of the C++ standard, hence the following changes are detectable:

- Classes
  - Removing/renaming a record type with external linkage
  - Adding/removing the `final` specifier of a class declaration
  - Adding/removing/reordering base classes
  - Change in class size
- Fields
  - Changing the type of a field
  - Changing the order of fields
  - Changing the access modifier of a field
  - Adding/removing the `static` specifier of a field
- Methods
  - Adding/remvoing the `const` specifier of a method
  - Adding/remvoing the `volatile` specifier of a method
  - Adding/remvoing the `static` specifier of a method
  - Adding/remvoing the `inline` specifier of a method
  - Chaning the signature of a method
  - Changing the ref-qualifier of a method
  - Changing the access modifier of a method
  - Moving the definition of a method into the body of its class
  - Adding/removing/reordering virtual methods in a class
  - Adding/remvoing the `final` specifier of a virtual method
  - Changing the virtuality of an existing function
- Functions
  - Removing/renaming a function with external linkage
  - Changing the linkage of a function
  - Adding/removing the `inline` specifier
  - Changing the signature of the function
  - Changing template arguments of an explicit function template specialization
- Global Variables
  - Removing/renaming a global variable with external linkage
  - Changing the linkage of a global variable
  - Changing the type of a global variable
