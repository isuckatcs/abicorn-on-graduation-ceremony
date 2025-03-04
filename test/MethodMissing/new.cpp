class c {
  int x() const { return 0; };
  int z() const { return 0; };

  c(float x){};
};

// CHECK: old.cpp:3:7: warning: method 'y' not found in the new library [method-checker]
// CHECK-NEXT:   int y() const { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT:old.cpp:6:3: warning: overload of 'c' not found in the new library [method-checker]
// CHECK-NEXT:   c(int x){};
// CHECK-NEXT:   ^
// CHECK-NEXT:new.cpp:5:3: note: found similar method [method-checker]
// CHECK-NEXT:   c(float x){};
// CHECK-NEXT:   ^
class c2 {
};
