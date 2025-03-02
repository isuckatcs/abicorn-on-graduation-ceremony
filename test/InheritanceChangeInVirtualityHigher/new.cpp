class A {};

class B : A {};

class C : B {};

// CHECK: new.cpp:3:11: warning: change in base class virtuality [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:3:19: note: 'A' is inherited virtually in the old library [inheritance-checker]
// CHECK-NEXT: class B : virtual A {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: new.cpp:3:11: note: 'A' is inherited non-virtually in the new library [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:5:11: warning: inheriting from class with modified base class hierarchy [inheritance-checker]   
// CHECK-NEXT: class C : B {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:5:11: note: the base class hierarchy of 'B' has changed in the new library [inheritance-checker]
