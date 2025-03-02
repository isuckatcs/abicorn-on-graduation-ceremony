class c{
virtual void foo();
void bar();
void baz();
};

// CHECK: new.cpp:2:14: warning: change in the virtuality of 'foo' [vtable-checker]
// CHECK-NEXT: virtual void foo();
// CHECK-NEXT:              ^
// CHECK-NEXT: old.cpp:2:6: note: function 'foo' is declared as 'non-virtual' in the old library [vtable-checker]
// CHECK-NEXT: void foo();
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:2:14: note: function 'foo' is declared as 'virtual' in the new library [vtable-checker]
// CHECK-NEXT: virtual void foo();
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:3:6: warning: change in the virtuality of 'bar' [vtable-checker]
// CHECK-NEXT: void bar();
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:3:14: note: function 'bar' is declared as 'virtual' in the old library [vtable-checker]
// CHECK-NEXT: virtual void bar();
// CHECK-NEXT:              ^
// CHECK-NEXT: new.cpp:3:6: note: function 'bar' is declared as 'non-virtual' in the new library [vtable-checker]
// CHECK-NEXT: void bar();
// CHECK-NEXT:      ^
