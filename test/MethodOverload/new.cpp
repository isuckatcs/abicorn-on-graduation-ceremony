class c {
  int x() const;
  void x(int, int) const;
  double x(int, float) const;
};

// CHECK: old.cpp:3:9: warning: overload of 'x' not found in the new library [method-checker]
// CHECK-NEXT:   float x(int) const;
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:2:7: note: found similar method [method-checker]
// CHECK-NEXT:   int x() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:3:8: note: found similar method [method-checker]
// CHECK-NEXT:   void x(int, int) const;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:4:10: note: found similar method [method-checker]
// CHECK-NEXT:   double x(int, float) const;
// CHECK-NEXT:          ^
