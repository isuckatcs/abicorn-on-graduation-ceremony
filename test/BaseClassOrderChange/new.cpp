class A {};
class B {};
class C {};

class D : public C, public A, public B {};

struct E : B, public A {};

struct F : E {};

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
// CHECK-NEXT: new.cpp:7:22: warning: change in base class order [inheritance-checker]
// CHECK-NEXT:  struct E : B, public A {};
// CHECK-NEXT:                       ^
// CHECK-NEXT: old.cpp:7:19: note: 'A' is at index 0 in the old library  [inheritance-checker]
// CHECK-NEXT:  struct E : public A, B {};
// CHECK-NEXT:                    ^
// CHECK-NEXT: new.cpp:7:22: note: 'A' is at index 1 in the new library  [inheritance-checker]
// CHECK-NEXT:  struct E : B, public A {};
// CHECK-NEXT:                       ^
// CHECK-NEXT: new.cpp:7:12: warning: change in base class order [inheritance-checker]
// CHECK-NEXT:  struct E : B, public A {};
// CHECK-NEXT:             ^
// CHECK-NEXT: old.cpp:7:22: note: 'B' is at index 1 in the old library  [inheritance-checker]
// CHECK-NEXT:  struct E : public A, B {};
// CHECK-NEXT:                       ^
// CHECK-NEXT: new.cpp:7:12: note: 'B' is at index 0 in the new library  [inheritance-checker]
// CHECK-NEXT:  struct E : B, public A {};
// CHECK-NEXT:             ^
// CHECK-NEXT: new.cpp:9:12: warning: inheriting from class with modified base class hierarchy [inheritance-checker]
// CHECK-NEXT:  struct F : E {};
// CHECK-NEXT:             ^