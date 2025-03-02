class C {
public:
  int x;
};

// CHECK: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the access modifier of 'x' [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'private' in the 'old' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'public' in the 'new' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^

class C2 {
protected:
  int x;
};

// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the access modifier of 'x' [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'public' in the 'old' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'protected' in the 'new' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^

class C3 {
private:
  int x;

  union {
    struct {
      int y;
    };
  };
};

// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in the access modifier of 'x' [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'protected' in the 'old' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: 'x' is declared 'private' in the 'new' library [field-checker]
// CHECK-NEXT:  int x;
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:37:11: warning: change in the access modifier of 'y' [field-checker]
// CHECK-NEXT:       int y;
// CHECK-NEXT:           ^
// CHECK-NEXT: old.cpp:17:11: note: 'y' is declared 'protected' in the 'old' library [field-checker]
// CHECK-NEXT:       int y;
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:37:11: note: 'y' is declared 'private' in the 'new' library [field-checker]
// CHECK-NEXT:       int y;
// CHECK-NEXT:           ^
