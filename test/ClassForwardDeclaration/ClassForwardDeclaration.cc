// RUN: abicorn --no-color --old old.cpp old2.cpp --new new.cpp new2.cpp -- 2>&1 | ( ! grep ^ )
