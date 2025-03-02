namespace {
class C;
} // namespace

class MissingOld {};

template <int> class T {};
template <> class T<0> {};

template <int, int> class P {};
template <int X> class P<0, X> {};
