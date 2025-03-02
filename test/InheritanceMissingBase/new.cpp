class A {};

class B {};

// CHECK: new.cpp:3:7: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: class B {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:3:11: note: 'B' is derived from 'A' in the old library [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:3:7: note: 'B' is not derived from 'A' in the new library [inheritance-checker]
// CHECK-NEXT: class B {};
// CHECK-NEXT:       ^
