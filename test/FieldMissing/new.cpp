class C {
public:
  int y;
};

// CHECK: old.cpp:2:7: warning: field 'x' is missing from the new library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:5:7: warning: field 'z' is missing from the new library [field-checker]
// CHECK-NEXT:  int z;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:3:7: warning: field 'y' is missing from the old library [field-checker]
// CHECK-NEXT:  int y;
// CHECK-NEXT:      ^
