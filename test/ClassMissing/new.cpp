void foo() {
    class C {};
}

class MissingNew {};

template <int> class T {};
template <> class T<1> {};

template <int, int> class P {};
template <int X> class P<1, X> {};

// CHECK: old.cpp:5:7: warning: 'MissingOld' is not defined in the new library [class-checker]
// CHECK-NEXT: class MissingOld {};
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:8:19: warning: specialization of 'T' is not defined in the new library [class-checker]
// CHECK-NEXT: template <> class T<0> {};
// CHECK-NEXT:                   ^
// CHECK-NEXT: old.cpp:11:24: warning: partial specialization of 'P' is not defined in the new library [class-checker]
// CHECK-NEXT: template <int X> class P<0, X> {};
// CHECK-NEXT:                        ^
