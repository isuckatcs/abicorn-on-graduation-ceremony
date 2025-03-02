struct S;

class C {
public:
  float x;
  S *y;

  union {
    struct {
      char z;
    };
  };
};
