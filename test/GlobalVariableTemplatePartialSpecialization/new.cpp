template<typename T, typename U, template<int> class C>
int X;

template<int>
struct A {};

template<typename U>
int X<int, U, A>;

// CHECK: old.cpp:5:8: warning: 'S' is not defined in the new library [class-checker]
// CHECK-NEXT: struct S {};
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp:8:5: warning: specialization of variable 'X' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int X<int, U, S>;
// CHECK-NEXT:     ^
