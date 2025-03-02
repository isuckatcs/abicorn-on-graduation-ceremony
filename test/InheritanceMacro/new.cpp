#define MACRO A2
#define MACRO_FN(X) X

class A {};

class A2 {};

class B : public MACRO {};

class C : protected MACRO_FN(A) {};

// CHECK: new.cpp:8:7: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:6:18: note: 'B' is derived from 'MACRO' in the old library [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:                  ^
// CHECK-NEXT: old.cpp:1:15: note: expanded from macro 'MACRO'
// CHECK-NEXT: #define MACRO A
// CHECK-NEXT:               ^
// CHECK-NEXT: new.cpp:8:7: note: 'B' is not derived from 'MACRO' in the new library [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:8:7: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:6:7: note: 'B' is not derived from 'MACRO' in the old library [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:8:18: note: 'B' is derived from 'MACRO' in the new library [inheritance-checker]
// CHECK-NEXT: class B : public MACRO {};
// CHECK-NEXT:                  ^
// CHECK-NEXT: new.cpp:1:15: note: expanded from macro 'MACRO'
// CHECK-NEXT: #define MACRO A2
// CHECK-NEXT:               ^
