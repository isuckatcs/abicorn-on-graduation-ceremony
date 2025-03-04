template <typename T, typename U> struct c {};

template <typename U> struct c<int, U> {
  void foo();
};

template <typename U> struct c<float, U> {
  void foo();
};

template <typename T> struct c2 {};

template <> struct c2<int> {
  void foo();
};

template <> struct c2<float> {
  void foo();
};
