class C {
  static int y;
  static int z;
};

// CHECK: old.cpp:2:14: warning: field 'x' is missing from the new library [field-checker]
// CHECK-NEXT:   static int x;
// CHECK-NEXT:              ^
// CHECK-NEXT: old.cpp:3:14: warning: field 'x2' is missing from the new library [field-checker]
// CHECK-NEXT:   static int x2;
// CHECK-NEXT:              ^
