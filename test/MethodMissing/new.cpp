class c {
  int x() const { return 0; };
  int z() const { return 0; };
};

// CHECK: old.cpp:3:7: warning: method 'y' not found in the new library [method-checker]
// CHECK-NEXT:   int y() const { return 0; };
// CHECK-NEXT:       ^
