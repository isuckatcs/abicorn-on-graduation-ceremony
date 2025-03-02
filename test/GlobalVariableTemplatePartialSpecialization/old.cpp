template<typename T, typename U, template<int> class C>
int X;

template<int>
struct S {};

template<typename U>
int X<int, U, S>;
