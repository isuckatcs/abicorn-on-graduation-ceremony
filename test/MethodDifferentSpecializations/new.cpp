template <int> class C {};
template <> class C<1> { void foo(); };
template <> class C<2> { int foo(); };

// CHECK-NOT: -
