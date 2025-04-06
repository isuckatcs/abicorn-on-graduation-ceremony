struct S {
  virtual void foo() &;
  virtual void foo() &&;

  virtual void bar();
  virtual void bar() const;

  virtual void baz() const;
  virtual void baz();
};
// CHECK: new.cpp:9:16: warning: change in virtual function index [vtable-checker]
// CHECK-NEXT:    virtual void baz();
// CHECK-NEXT:                 ^
// CHECK-NEXT: old.cpp:8:16: note: virtual function 'baz' is at index 4 in the old library [vtable-checker]
// CHECK-NEXT:    virtual void baz();
// CHECK-NEXT:                 ^
// CHECK-NEXT: new.cpp:9:16: note: virtual function 'baz' is at index 5 in the new library [vtable-checker]
// CHECK-NEXT:    virtual void baz();
// CHECK-NEXT:                 ^
// CHECK-NEXT: new.cpp:8:16: warning: change in virtual function index [vtable-checker]
// CHECK-NEXT:    virtual void baz() const;
// CHECK-NEXT:                 ^
// CHECK-NEXT: old.cpp:9:16: note: virtual function 'baz' is at index 5 in the old library [vtable-checker]
// CHECK-NEXT:    virtual void baz() const;
// CHECK-NEXT:                 ^
// CHECK-NEXT: new.cpp:8:16: note: virtual function 'baz' is at index 4 in the new library [vtable-checker]
// CHECK-NEXT:    virtual void baz() const;
// CHECK-NEXT:                 ^
// CHECK-NOT: {{.*}}