template<typename T>
class c {
  int x;

  class nested {
    T x();
  };
};

class d {
  int y;

  template<typename T>
  class nested {
    T y();
  };
};

// CHECK: new.cpp:6:7: warning: change in the 'const' qualifier of 'x' [method-checker]
// CHECK-NEXT:     T x();
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:6:7: note: 'x' is 'const' in the old library [method-checker]
// CHECK-NEXT:     T x() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:6:7: note: 'x' is not 'const' in the new library [method-checker]
// CHECK-NEXT:     T x();
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:15:7: warning: change in the 'const' qualifier of 'y' [method-checker]
// CHECK-NEXT:     T y();
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:15:7: note: 'y' is 'const' in the old library [method-checker]
// CHECK-NEXT:     T y() const;
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:15:7: note: 'y' is not 'const' in the new library [method-checker]
// CHECK-NEXT:     T y();
// CHECK-NEXT:       ^
