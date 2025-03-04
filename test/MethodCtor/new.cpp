class c {};

class c2 {};
// CHECK: old.cpp:6:3: warning: method 'c2' not found in the new library [method-checker]
// CHECK-NEXT:    c2(int) {}
// CHECK-NEXT:    ^

class c3 {
  c3(int) {}
};
// CHECK-NEXT: old.cpp:10:3: warning: overload of 'c3' not found in the new library [method-checker]
// CHECK-NEXT:    c3() {}
// CHECK-NEXT:    ^
// CHECK-NEXT: new.cpp:[[# @LINE - 5 ]]:3: note: found similar method [method-checker]
// CHECK-NEXT:    c3(int) {}
// CHECK-NEXT:    ^
class c4 {
  c4() {}
  c4(int) {}
};

class c5 {
  c5(int){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 3 ]]:7: warning: the implicit default constructor of 'c5' is removed in the new library [method-checker]
// CHECK-NEXT:  class c5 {
// CHECK-NEXT:        ^

class c6 {
  c6(){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: change in the access modifier of method 'c6' [method-checker]
// CHECK-NEXT:    c6(){};
// CHECK-NEXT:    ^
// CHECK-NEXT: old.cpp:21:3: note: method 'c6' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    c6() {}
// CHECK-NEXT:    ^
// CHECK-NEXT: new.cpp:[[# @LINE - 8 ]]:3: note: method 'c6' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:    c6(){};
// CHECK-NEXT:    ^

struct s1 {
private:
  s1(){};
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: the default constructor of 's1' is no longer public the new library [method-checker]
// CHECK-NEXT:    s1(){};
// CHECK-NEXT:    ^

class c7 {
  c7() = delete;
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: method 'c7' is marked deleted in the new library [method-checker]
// CHECK-NEXT:    c7() = delete;
// CHECK-NEXT:    ^
// CHECK-NEXT: old.cpp:27:3: note: method 'c7' is not marked deleted in the old library [method-checker]
// CHECK-NEXT:    c7() {}
// CHECK-NEXT:    ^

class c8 {
  c8() = delete;
};
// CHECK-NEXT: new.cpp:[[# @LINE - 2 ]]:3: warning: the default constructor of 'c8' is deleted in the new library [method-checker]
// CHECK-NEXT:    c8() = delete;
// CHECK-NEXT:    ^
