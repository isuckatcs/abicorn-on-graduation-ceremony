static void reachable() {};
// CHECK: old.cpp:1:6: warning: 'reachable' is not defined in the new library [function-checker]
// CHECK-NEXT: void reachable() {};
// CHECK-NEXT:      ^
void unreachable() {};
