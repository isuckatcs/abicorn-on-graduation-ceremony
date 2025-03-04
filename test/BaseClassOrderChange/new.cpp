class A {};
class B {};
class C {};

class D : public C, public A, public B {};
// CHECK: new.cpp:5:28: warning: change in base class order [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                             ^
// CHECK-NEXT: old.cpp:5:18: note: 'A' is at index 0 in the old library  [inheritance-checker]
// CHECK-NEXT:  class D : public A, public B, public C {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: new.cpp:5:28: note: 'A' is at index 1 in the new library  [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                             ^
// CHECK-NEXT: new.cpp:5:38: warning: change in base class order [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                                       ^
// CHECK-NEXT: old.cpp:5:28: note: 'B' is at index 1 in the old library  [inheritance-checker]
// CHECK-NEXT:  class D : public A, public B, public C {};
// CHECK-NEXT:                             ^
// CHECK-NEXT: new.cpp:5:38: note: 'B' is at index 2 in the new library  [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                                       ^
// CHECK-NEXT: new.cpp:5:18: warning: change in base class order [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: old.cpp:5:38: note: 'C' is at index 2 in the old library  [inheritance-checker]
// CHECK-NEXT:  class D : public A, public B, public C {};
// CHECK-NEXT:                                       ^
// CHECK-NEXT: new.cpp:5:18: note: 'C' is at index 0 in the new library  [inheritance-checker]
// CHECK-NEXT:  class D : public C, public A, public B {};
// CHECK-NEXT:                   ^
