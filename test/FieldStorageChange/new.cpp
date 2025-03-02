class C {
  int x;
  static int y;
};

//CHECK: new.cpp:2:7: warning: change in the storage of 'x' [field-checker]
//CHECK-NEXT:  int x;
//CHECK-NEXT:      ^
//CHECK-NEXT: old.cpp:2:14: note: 'x' has static storage in the old library [field-checker]
//CHECK-NEXT:  static int x;
//CHECK-NEXT:             ^
//CHECK-NEXT: new.cpp:2:7: note: 'x' has non-static storage in the new library [field-checker]
//CHECK-NEXT:  int x;
//CHECK-NEXT:      ^
//CHECK-NEXT: new.cpp:3:14: warning: change in the storage of 'y' [field-checker]
//CHECK-NEXT:  static int y;
//CHECK-NEXT:             ^
//CHECK-NEXT: old.cpp:3:7: note: 'y' has non-static storage in the old library [field-checker]
//CHECK-NEXT:  int y;
//CHECK-NEXT:      ^
//CHECK-NEXT: new.cpp:3:14: note: 'y' has static storage in the new library [field-checker]
//CHECK-NEXT:  static int y;
//CHECK-NEXT:             ^
