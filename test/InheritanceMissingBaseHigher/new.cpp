class A {};

class B {};

class C : B {};

// CHECK: new.cpp:3:7: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: class B {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:3:11: note: 'B' is derived from 'A' in the old library [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:3:7: note: 'B' is not derived from 'A' in the new library [inheritance-checker]
// CHECK-NEXT: class B {};
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:5:11: warning: inheriting from class with modified base class hierarchy [inheritance-checker]
// CHECK-NEXT: class C : B {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:5:11: note: the base class hierarchy of 'B' has changed in the new library [inheritance-checker]
