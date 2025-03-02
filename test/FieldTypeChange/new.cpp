struct S2 {};

class C {
public:
  int x;
  S2 y;

  union {
    struct {
      int z;
    };
  };
};

// CHECK: new.cpp:5:7: warning: change in the type of 'x' [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:5:9: note: the type of 'x' is 'float' in the old library [field-checker]     
// CHECK-NEXT:  float x;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:5:7: note: the type of 'x' is 'int' in the new library [field-checker]       
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:6:6: warning: change in the type of 'y' [field-checker]
// CHECK-NEXT:  S2 y;
// CHECK-NEXT:     ^
// CHECK-NEXT: old.cpp:6:6: note: the type of 'y' is 'S *' in the old library [field-checker]
// CHECK-NEXT:  S *y;
// CHECK-NEXT:     ^
// CHECK-NEXT: new.cpp:6:6: note: the type of 'y' is 'S2' in the new library [field-checker] 
// CHECK-NEXT:  S2 y;
// CHECK-NEXT:     ^
// CHECK-NEXT: new.cpp:10:11: warning: change in the type of 'z' [field-checker]
// CHECK-NEXT:       int z;
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:10:12: note: the type of 'z' is 'char' in the old library [field-checker]
// CHECK-NEXT:       char z;
// CHECK-NEXT:            ^
// CHECK-NEXT: new.cpp:10:11: note: the type of 'z' is 'int' in the new library [field-checker]
// CHECK-NEXT:       int z;
// CHECK-NEXT:           ^
