class A {};

template <int, int> class B {};

template <int X> class B<0, X> {};

class C : B<0, 1> {};

template <typename T> class D {};

class E : D<int> {};