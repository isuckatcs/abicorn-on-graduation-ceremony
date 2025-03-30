struct c {
  int x() const { return 0; };
  int y() const { return 0; };
  int deleted();

  c(){};
  c(int x){};
};

struct c2 {
  c2(){};
};
