static int Reachable;
// CHECK: old.cpp:1:5: warning: 'Reachable' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int Reachable;
// CHECK-NEXT:     ^

int Unreachable;