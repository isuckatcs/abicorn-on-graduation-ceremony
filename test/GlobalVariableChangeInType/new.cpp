float X;

// CHECK: new.cpp:1:7: warning: change in the type of 'X' [global-variable-checker]
// CHECK-NEXT: float X;
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:1:5: note: the type of 'X' is 'int' in the old library [global-variable-checker]
// CHECK-NEXT: int X;
// CHECK-NEXT:     ^
// CHECK-NEXT: new.cpp:1:7: note: the type of 'X' is 'float' in the new library [global-variable-checker]
// CHECK-NEXT: float X;
// CHECK-NEXT:       ^
