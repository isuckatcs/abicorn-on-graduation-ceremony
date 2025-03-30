template<typename T>
class c {
  int x;

  struct nested {
    T x() const;
  };
};

class d {
  int y;

  template<typename T>
  struct nested {
    T y() const;
  };
};
