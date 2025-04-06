namespace v1 {
struct Param {};
} // namespace v1

namespace v2 {
struct Param {};
} // namespace v2

struct S {
  S(const v1::Param *P);
  S(const v2::Param *P);
};

template <class T> struct S2 {};

template <> struct S2<v1::Param> {
  static void mapping(v1::Param);
};
// CHECK: old.cpp:18:15: warning: overload of 'mapping' not found in the new library [method-checker]
// CHECK-NEXT:    static void mapping(v1::Param, int);
// CHECK-NEXT:                ^
// CHECK-NEXT: new.cpp:[[# @LINE - 5 ]]:15: note: found similar method [method-checker]
// CHECK-NEXT:    static void mapping(v1::Param);
// CHECK-NEXT:                ^
template <> struct S2<v2::Param> {
  static void mapping(v2::Param);
};
// CHECK-NEXT: old.cpp:23:15: warning: overload of 'mapping' not found in the new library [method-checker]
// CHECK-NEXT:    static void mapping(v2::Param, int);
// CHECK-NEXT:                ^
// CHECK-NEXT: new.cpp:[[# @LINE - 5 ]]:15: note: found similar method [method-checker]
// CHECK-NEXT:    static void mapping(v2::Param);
// CHECK-NEXT:                ^

template <class T> struct S3 {};

auto L = []() {};

template <> struct S3<decltype(L)> {};

template <typename T> struct TT {};

struct S4 {
  auto foo() {
    auto L = []() {};
    return TT<decltype(L)>{};
  }
};

// CHECK-NOT: {{.*}}
