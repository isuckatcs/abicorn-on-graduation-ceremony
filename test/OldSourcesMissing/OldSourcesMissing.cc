// RUN: (abicorn --new source.cpp || true) 2>&1 | filecheck %s
// CHECK: Error: old input files not specified
