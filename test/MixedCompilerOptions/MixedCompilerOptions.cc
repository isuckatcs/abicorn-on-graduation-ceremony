// RUN: abicorn --no-color --old old.cpp -s-old=-std=c++98 --new new.cpp -s-new=-std=c++11 -- -DCOMMON_COMPILER_OPTION 2>&1 | ( ! grep ^ )
