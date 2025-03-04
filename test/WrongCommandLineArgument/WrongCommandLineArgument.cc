// RUN: (abicorn --unknown || true) 2>&1 | filecheck %s
// CHECK: Unknown command line argument '--unknown'.
