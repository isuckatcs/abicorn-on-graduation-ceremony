template <typename T, typename U> struct c {};

template <typename U> struct c<int, U> {
private:
  void foo();
};

template <typename U> struct c<float, U> {
protected:
  void foo();
};

template <typename T> struct c2 {};

template <> struct c2<int> {
private:
  void foo();
};

template <> struct c2<float> {
protected:
  void foo();
};
// CHECK-NEXT: new.cpp:5:8: warning: change in the access modifier of method 'foo' [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:4:8: note: method 'foo' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:5:8: note: method 'foo' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:10:8: warning: change in the access modifier of method 'foo' [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:8:8: note: method 'foo' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:10:8: note: method 'foo' is declared 'protected' in the new library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:17:8: warning: change in the access modifier of method 'foo' [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:14:8: note: method 'foo' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:17:8: note: method 'foo' is declared 'private' in the new library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:22:8: warning: change in the access modifier of method 'foo' [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: old.cpp:18:8: note: method 'foo' is declared 'public' in the old library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^
// CHECK-NEXT: new.cpp:22:8: note: method 'foo' is declared 'protected' in the new library [method-checker]
// CHECK-NEXT:    void foo();
// CHECK-NEXT:         ^