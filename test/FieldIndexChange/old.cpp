class C {
  int x;
  int y;
};

class C2 {
  static int x;
  static int y;
};

class C3 {
  static int x;
  int y;
};

class C4 {
  int x;

  union {
    struct {
      int r;
      int g;
      int b;
    };
  };
};

class C5 {
  union {
    int x;
    int y;
    int z;
  };

  int dummy;
  int dummy2;

  union {
    int r;
    int g;
    int b;
  };
};
