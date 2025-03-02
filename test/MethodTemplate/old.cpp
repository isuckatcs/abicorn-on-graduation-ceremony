template<typename T>
class c {
  T x() const;
  void y();
};

template<>
class c<int>{
  int x() const;
  void y(int);
};
