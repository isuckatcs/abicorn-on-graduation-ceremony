class c {
public:
  int x() const { return 0; };
  int x2() { return 1; }

private: 
  int y() const;

protected:  
  float z() { return 2.f; }

public:
  void leak() { y(); }
};

// CHECK: new.cpp:3:7: warning: change in the 'inline' qualifier of 'x' [method-checker]
// CHECK-NEXT:   int x() const { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:3:7: note: 'x' is not 'inline' in the old library [method-checker]
// CHECK-NEXT:   int x() const
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:3:7: note: 'x' is 'inline' in the new library [method-checker]
// CHECK-NEXT:   int x() const { return 0; };
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:4:7: warning: change in the 'volatile' qualifier of 'x2' [method-checker]
// CHECK-NEXT:   int x2() { return 1; }
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:4:7: note: 'x2' is 'volatile' in the old library [method-checker]
// CHECK-NEXT:   int x2() volatile { return 1; }
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:4:7: note: 'x2' is not 'volatile' in the new library [method-checker]
// CHECK-NEXT:   int x2() { return 1; }
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:7:7: warning: change in the 'const' qualifier of 'y' [method-checker]
// CHECK-NEXT:   int y() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:7:14: note: 'y' is not 'const' in the old library [method-checker]
// CHECK-NEXT:   inline int y() { return 1; }
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:7:7: note: 'y' is 'const' in the new library [method-checker]
// CHECK-NEXT:   int y() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:7:7: warning: change in the 'inline' qualifier of 'y' [method-checker]
// CHECK-NEXT:   int y() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:7:14: note: 'y' is 'inline' in the old library [method-checker]
// CHECK-NEXT:   inline int y() { return 1; }
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:7:7: note: 'y' is not 'inline' in the new library [method-checker]
// CHECK-NEXT:   int y() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:10:9: warning: change in the 'static' qualifier of 'z' [method-checker]
// CHECK-NEXT:   float z() { return 2.f; }
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:10:16: note: 'z' is 'static' in the old library [method-checker]
// CHECK-NEXT:   static float z() { return 2.f; }
// CHECK-NEXT:                ^
// CHECK-NEXT: new.cpp:10:9: note: 'z' is not 'static' in the new library [method-checker]
// CHECK-NEXT:   float z() { return 2.f; }
// CHECK-NEXT:         ^
// CHECK-NOT: {{.*}}
