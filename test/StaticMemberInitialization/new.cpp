class C {
  static float x;
};

float C::x = 1;
// CHECK: new.cpp:2:16: warning: change in the type of 'x' [field-checker]
// CHECK-NEXT:    static float x;
// CHECK-NEXT:                 ^
// CHECK-NEXT: old.cpp:2:14: note: the type of 'x' is 'int' in the old library [field-checker]
// CHECK-NEXT:    static int x;
// CHECK-NEXT:               ^
// CHECK-NEXT: new.cpp:2:16: note: the type of 'x' is 'float' in the new library [field-checker]
// CHECK-NEXT:    static float x;
// CHECK-NEXT:                 ^
// CHECK-NOT: {{.*}}