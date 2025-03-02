class c {
};

class d {
  d();
  d(int, double);
};

// CHECK: old.cpp:2:3: warning: method 'c' not found in the new library [method-checker]
// CHECK-NEXT:   c(int);
// CHECK-NEXT:   ^
// CHECK-NEXT: old.cpp:6:3: warning: overload of 'd' not found in the new library [method-checker]
// CHECK-NEXT:   d(int, float);
// CHECK-NEXT:   ^
// CHECK-NEXT: new.cpp:5:3: note: found similar method [method-checker]
// CHECK-NEXT:   d();
// CHECK-NEXT:   ^
// CHECK-NEXT: new.cpp:6:3: note: found similar method [method-checker]
// CHECK-NEXT:   d(int, double);
// CHECK-NEXT:   ^
