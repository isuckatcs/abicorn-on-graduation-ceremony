class c {
private:
  int x() { return 0; };

protected:
  int y() { return 1; };

public:
  float z() { return 2.f; }

public:
  void leak() { y(); }
};

// CHECK: new.cpp:3:7: warning: change in the access modifier of method 'x' [method-checker]
// CHECK-NEXT:   int x() { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:3:7: note: method 'x' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:   int x() { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:3:7: note: method 'x' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:   int x() { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:6:7: warning: change in the access modifier of method 'y' [method-checker]
// CHECK-NEXT:   int y() { return 1; };
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:6:7: note: method 'y' is declared 'private' in the old library [method-checker]
// CHECK-NEXT:   int y() { return 1; };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:6:7: note: method 'y' is declared 'protected' in the new library [method-checker]
// CHECK-NEXT:   int y() { return 1; };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:9:9: warning: change in the access modifier of method 'z' [method-checker]
// CHECK-NEXT:   float z() { return 2.f; }
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:9:9: note: method 'z' is declared 'protected' in the old library [method-checker]
// CHECK-NEXT:   float z() { return 2.f; }
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:9:9: note: method 'z' is declared 'public' in the new library [method-checker]
// CHECK-NEXT:   float z() { return 2.f; }
// CHECK-NEXT:         ^
// CHECK-NOT: {{.*}}
