// RUN: (abicorn --old source.cpp || true) 2>&1 | filecheck %s
// CHECK: Error: new input files not specified
