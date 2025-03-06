<div align="center">
<img src="https://github.com/isuckatcs/abicorn-for-graduation/actions/workflows/pipeline.yml/badge.svg" />
<a href="https://codecov.io/github/isuckatcs/abicorn-for-graduation" target="_blank"><img src="https://codecov.io/github/isuckatcs/abicorn-for-graduation/graph/badge.svg?token=ZWYOPB837H" /></a>
<br>
<br>
</div>

![cover](img/definitely_not_ai_generated_cover.jpg)

It's exactly like those graduation gift balloons, except this is a Clang Tool that detects library-level API and ABI compatibility breaking changes based on source code.

## 🦄 Get Started 🦄

This guide assumes that the project is being built on Linux but equivalent steps can be performed on any other operating system.


1. Install CMake 3.13.4 or newer
    ```
    apt-get install cmake
    ```

2. Install LLVM and Clang
    ```
    apt-get install llvm clang
    ```
    The project requires **LLVM 17.0.0**. Later versions are not guaranteed to work, while older versions are guaranteed to not work.

3. Create a build directory and enter that directory
    ```
    mkdir build && cd build
    ```

4. Configure and build the project
   ```
   cmake path/to/repo/root && cmake --build .
   ```

To run the tests, proceed with these following optional steps.

5. Install Python 3.10 or newer
    ```
    apt-get install python
    ```

6. Install the required dependencies
    ```
    pip install -r ./test/requirements.txt
    ```

7. Run the `test` target with CMake
    ```
    cmake --build . --target test
    ```
To run the unit tests, proceed with the following still optional steps.

8. Clone the third-party dependencies
    ```
    git submodule init && git submodule update
    ```

9. Enable building the unit tests with CMake
    ```
    cmake -DBUILD_UNIT_TESTS=On .
    ```

10. Run the `unit` target with CMake
    ```
    cmake --build . --target unit
    ```

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
  - Removing class template specializations with external linkage
  - Adding/removing the `final` specifier of a class declaration
  - Adding/removing/reordering base classes
  - Changing the virtuality of base classes
  - Changing inheritance access specifiers
  - Changing the size of the class
- Fields
  - Adding/removing fields
  - Changing the type of a field
  - Changing the order of fields
  - Changing the access modifier of a field
  - Adding/removing the `static` specifier of a field
- Methods
  - Removing methods
  - Adding/remvoing the `const` specifier of a method
  - Adding/remvoing the `volatile` specifier of a method
  - Adding/remvoing the `static` specifier of a method
  - Adding/remvoing the `inline` specifier of a method
  - Marking a method deleted
  - Chaning the signature of a method
  - Changing the ref-qualifier of a method
  - Changing the access modifier of a method
  - Moving the definition of a method into the body of its class
  - Adding/removing/reordering virtual methods
  - Adding/remvoing the `final` specifier of a virtual method
  - Changing the virtuality of an existing function
- Functions
  - Removing/renaming a function with external linkage
  - Removing function template specializations with external linkage
  - Changing the linkage of a function
  - Adding/removing the `inline` specifier
  - Changing the signature of the function
- Global Variables
  - Removing/renaming a global variable with external linkage
  - Removing template specializations of global variables with external linkage
  - Changing the linkage of a global variable
  - Changing the type of a global variable
