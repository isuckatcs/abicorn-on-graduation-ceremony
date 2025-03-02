class C {
  int x;

  class N {
    const int x = 0;
  };
};

// CHECK: new.cpp:5:15: warning: change in the type of 'x' [field-checker]
// CHECK-NEXT:     const int x = 0;
// CHECK-NEXT:               ^
// CHECK-NEXT: old.cpp:5:9: note: the type of 'x' is 'int' in the old library [field-checker]
// CHECK-NEXT:     int x;
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:5:15: note: the type of 'x' is 'const int' in the new library [field-checker]
// CHECK-NEXT:     const int x = 0;
// CHECK-NEXT:               ^
