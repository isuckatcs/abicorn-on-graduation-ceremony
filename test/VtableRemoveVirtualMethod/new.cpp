class c{
virtual void foo();
};

// CHECK: old.cpp:3:14: warning: virtual method 'bar' not found in the new library [vtable-checker]
// CHECK-NEXT: virtual void bar();
// CHECK-NEXT:              ^
