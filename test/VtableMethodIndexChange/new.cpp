class c{
virtual void bar();
virtual void foo();
};

// CHECK: new.cpp:3:14: warning: change in virtual function index [vtable-checker]
// CHECK-NEXT: virtual void foo();
// CHECK-NEXT:              ^
// CHECK-NEXT: old.cpp:2:14: note: virtual function 'foo' is at index 0 in the old library [vtable-checker]
// CHECK-NEXT: virtual void foo();
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:3:14: note: virtual function 'foo' is at index 1 in the new library [vtable-checker]
// CHECK-NEXT: virtual void foo();
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:2:14: warning: change in virtual function index [vtable-checker]
// CHECK-NEXT: virtual void bar();
// CHECK-NEXT:              ^
// CHECK-NEXT: old.cpp:3:14: note: virtual function 'bar' is at index 1 in the old library [vtable-checker]
// CHECK-NEXT: virtual void bar();
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:2:14: note: virtual function 'bar' is at index 0 in the new library [vtable-checker]
// CHECK-NEXT: virtual void bar();
// CHECK-NEXT:              ^
