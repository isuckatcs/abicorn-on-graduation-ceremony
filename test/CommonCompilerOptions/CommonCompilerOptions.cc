// RUN: abicorn --no-color --old old.cpp --new new.cpp -- -DCOMMON_COMPILER_OPTION 2>&1 | ( ! grep ^ )
