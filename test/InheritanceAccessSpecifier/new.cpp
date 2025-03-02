class A {};

class B : public A {};

//CHECK: new.cpp:3:18: warning: change in inheritance access specifier [inheritance-checker]
//CHECK-NEXT: class B : public A {};
//CHECK-NEXT:                  ^
//CHECK-NEXT: old.cpp:3:11: note: 'A' is inherited as 'private' in the 'old' library [inheritance-checker]
//CHECK-NEXT: class B : A {};
//CHECK-NEXT:           ^
//CHECK-NEXT: new.cpp:3:18: note: 'A' is inherited as 'public' in the 'new' library [inheritance-checker]
//CHECK-NEXT: class B : public A {};
//CHECK-NEXT:                  ^
