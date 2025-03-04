class c {
  ~c() {}
};
// CHECK: new.cpp:[[# @LINE - 2 ]]:3: warning: change in the access modifier of method '~c' [method-checker]
// CHECK-NEXT:    ~c() {}
// CHECK-NEXT:    ^
// CHECK-NEXT: old.cpp:3:3: note: method '~c' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    ~c() {}
// CHECK-NEXT:    ^
// CHECK-NEXT: new.cpp:[[# @LINE - 8 ]]:3: note: method '~c' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:    ~c() {}
// CHECK-NEXT:    ^
class c2 final {
  virtual ~c2() final {}
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:11: warning: change in virtual function finality [vtable-checker]
// CHECK-NEXT:    virtual ~c2() final {}
// CHECK-NEXT:            ^
// CHECK-NEXT: old.cpp:7:11: note: virtual function '~c2' is non-final in the old library [vtable-checker]
// CHECK-NEXT:    virtual ~c2() {}
// CHECK-NEXT:            ^
// CHECK-NEXT: new.cpp:[[# @LINE - 8 ]]:11: note: virtual function '~c2' is final in the new library [vtable-checker]
// CHECK-NEXT:    virtual ~c2() final {}
// CHECK-NEXT:            ^

class c3 {
  virtual ~c3() {}
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:11: warning: change in the virtuality of '~c3' [vtable-checker]
// CHECK-NEXT:    virtual ~c3() {}
// CHECK-NEXT:            ^
// CHECK-NEXT: old.cpp:11:3: note: function '~c3' is declared as 'non-virtual' in the old library [vtable-checker]
// CHECK-NEXT:    ~c3() {}
// CHECK-NEXT:    ^
// CHECK-NEXT: new.cpp:[[# @LINE - 8 ]]:11: note: function '~c3' is declared as 'virtual' in the new library [vtable-checker]
// CHECK-NEXT:    virtual ~c3() {}
// CHECK-NEXT:            ^

class c4 {};
// CHECK-NEXT: old.cpp:15:11: warning: virtual method '~c4' not found in the new library [vtable-checker]
// CHECK-NEXT:    virtual ~c4(){};
// CHECK-NEXT:            ^

class c5 {
  virtual ~c5(){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:11: warning: virtual method '~c5' not found in the old library [vtable-checker]
// CHECK-NEXT:    virtual ~c5(){};
// CHECK-NEXT:            ^

class c6 {
  ~c6(){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: the implicit destructor of 'c6' is replaced with a non-public destructor in the new library [method-checker]
// CHECK-NEXT:    ~c6(){};
// CHECK-NEXT:    ^

class c7 {
  virtual ~c7(){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:11: warning: change in the access modifier of method '~c7' [method-checker]
// CHECK-NEXT:    virtual ~c7(){};
// CHECK-NEXT:            ^
// CHECK-NEXT: old.cpp:24:11: note: method '~c7' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    virtual ~c7(){};
// CHECK-NEXT:            ^
// CHECK-NEXT: new.cpp:[[# @LINE - 8 ]]:11: note: method '~c7' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:    virtual ~c7(){};
// CHECK-NEXT:            ^

class c8 {
public:
  ~c8() = default;
};

class c9 {
  ~c9() = default;
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: the implicit destructor of 'c9' is replaced with a non-public destructor in the new library [method-checker]
// CHECK-NEXT:    ~c9() = default;
// CHECK-NEXT:    ^

class c10 {
public:
  ~c10() = delete;
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: the implicit destructor of 'c10' is deleted in the new library [method-checker]
// CHECK-NEXT:    ~c10() = delete;
// CHECK-NEXT:    ^

class c11 {};
