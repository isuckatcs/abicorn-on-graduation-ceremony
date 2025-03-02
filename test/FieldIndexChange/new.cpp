class C {
  int y;
  int x;
};

// CHECK: new.cpp:3:7: warning: change in the field index of 'x' [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:2:7: note: field 'x' is at index 0 in the old library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:3:7: note: field 'x' is at index 1 in the new library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:2:7: warning: change in the field index of 'y' [field-checker]
// CHECK-NEXT:  int y;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:3:7: note: field 'y' is at index 1 in the old library [field-checker]
// CHECK-NEXT:  int y;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:2:7: note: field 'y' is at index 0 in the new library [field-checker]
// CHECK-NEXT:  int y;
// CHECK-NEXT:      ^

class C2 {
  static int y;
  static int x;
};

class C3 {
  int y;
  static int x;
};

class C4 {
  union {
    struct {
      int g;
      int r;
      int b;
    };
  };

  int x;
};
// CHECK-NEXT: new.cpp:44:7: warning: change in the field index of 'x' [field-checker]
// CHECK-NEXT:   int x;
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:17:7: note: field 'x' is at index 0 in the old library [field-checker]
// CHECK-NEXT:   int x;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:44:7: note: field 'x' is at index 1 in the new library [field-checker]
// CHECK-NEXT:   int x;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:39:11: warning: change in the field index of 'r' [field-checker]
// CHECK-NEXT:       int r;
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:21:11: note: field 'r' is at index 0 in the old library [field-checker]
// CHECK-NEXT:       int r;
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:39:11: note: field 'r' is at index 1 in the new library [field-checker]
// CHECK-NEXT:       int r;
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:38:11: warning: change in the field index of 'g' [field-checker]
// CHECK-NEXT:       int g;
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:22:11: note: field 'g' is at index 1 in the old library [field-checker]
// CHECK-NEXT:       int g;
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:38:11: note: field 'g' is at index 0 in the new library [field-checker]
// CHECK-NEXT:       int g;
// CHECK-NEXT:           ^

class C5 {
  union {
    int x;
    int g;
    int z;
  };

  int dummy;
  int dummy2;

  union {
    int r;
    int y;
    int b;
  };
};
// CHECK-NEXT: new.cpp:86:9: warning: change in the field index of 'y' [field-checker]
// CHECK-NEXT:     int y;
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:31:9: note: field 'y' is at index 0 in the old library [field-checker]
// CHECK-NEXT:     int y;
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:86:9: note: field 'y' is at index 3 in the new library [field-checker]
// CHECK-NEXT:     int y;
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:77:9: warning: change in the field index of 'g' [field-checker]
// CHECK-NEXT:     int g;
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:40:9: note: field 'g' is at index 3 in the old library [field-checker]
// CHECK-NEXT:     int g;
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:77:9: note: field 'g' is at index 0 in the new library [field-checker]
// CHECK-NEXT:     int g;
// CHECK-NEXT:         ^
