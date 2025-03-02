class c{
void foo();
void bar();
virtual void vfoo();
int nfoo();
};

// CHECK: new.cpp:4:14: warning: virtual method 'vfoo' not found in the old library [vtable-checker]
// CHECK-NEXT: virtual void vfoo();
// CHECK-NEXT:              ^
