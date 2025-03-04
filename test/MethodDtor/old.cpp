class c {
public:
  ~c() {}
};

class c2 final {
  virtual ~c2() {}
};

class c3 {
  ~c3() {}
};

class c4 {
  virtual ~c4(){};
};

class c5 {};

class c6 {};

class c7 {
public:
  virtual ~c7(){};
};

class c8 {};

class c9 {};

class c10 {};

class c11 {
  ~c11(){};
};
