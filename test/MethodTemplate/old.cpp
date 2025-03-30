template<typename T>
struct c {
  T x() const;
  void y();
};

template<>
struct c<int>{
  int x() const;
  void y(int);
};
