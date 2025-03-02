class A {};

class B : A {};

class C : virtual A {};

// CHECK: new.cpp:3:11: warning: change in base class virtuality [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:3:19: note: 'A' is inherited virtually in the old library [inheritance-checker]
// CHECK-NEXT: class B : virtual A {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: new.cpp:3:11: note: 'A' is inherited non-virtually in the new library [inheritance-checker]
// CHECK-NEXT: class B : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:5:19: warning: change in base class virtuality [inheritance-checker]
// CHECK-NEXT: class C : virtual A {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: old.cpp:5:11: note: 'A' is inherited non-virtually in the old library [inheritance-checker]
// CHECK-NEXT: class C : A {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:5:19: note: 'A' is inherited virtually in the new library [inheritance-checker]
// CHECK-NEXT: class C : virtual A {};
// CHECK-NEXT:                   ^
