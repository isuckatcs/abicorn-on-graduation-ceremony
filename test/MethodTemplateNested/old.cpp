template<typename T>
class c {
  int x;

  class nested {
    T x() const;
  };
};

class d {
  int y;

  template<typename T>
  class nested {
    T y() const;
  };
};
