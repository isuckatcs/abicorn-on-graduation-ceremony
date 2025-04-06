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
  static void mapping(v1::Param, int);
};

template <> struct S2<v2::Param> {
  static void mapping(v2::Param);
  static void mapping(v2::Param, int);
};

template <class T> struct S3 {};

auto L = []() {};

template <> struct S3<decltype(L)> {};

