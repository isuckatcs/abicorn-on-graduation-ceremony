// RUN: python3 generate_compile_commands.py
// RUN: abicorn --no-color --old old.cpp -p-old="./build_old" --new new.cpp -p-new="./build_new" 2>&1 | count 0
