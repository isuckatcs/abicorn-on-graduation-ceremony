class A {};

class B {
  class A {};
};

// CHECK: new.cpp:4:9: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT:   class A {};
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:4:13: note: 'A' is derived from '::A' in the old library [inheritance-checker]
// CHECK-NEXT:   class A : ::A {};
// CHECK-NEXT:             ^
// CHECK-NEXT: new.cpp:4:9: note: 'A' is not derived from '::A' in the new library [inheritance-checker]
// CHECK-NEXT:   class A {};
// CHECK-NEXT:         ^
