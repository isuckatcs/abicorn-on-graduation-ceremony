class c {
  int x() { return 0; }
  
  class nested{
    int x() { return 0; }
    int x(int a) { return 0; }
  };
};

// CHECK: new.cpp:5:9: warning: change in the 'static' qualifier of 'x' [method-checker]
// CHECK-NEXT:     int x() { return 0; }
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:5:16: note: 'x' is 'static' in the old library [method-checker]
// CHECK-NEXT:     static int x() { return 0; }
// CHECK-NEXT:                ^
// CHECK-NEXT: new.cpp:5:9: note: 'x' is not 'static' in the new library [method-checker]
// CHECK-NEXT:     int x() { return 0; }
// CHECK-NEXT:         ^
