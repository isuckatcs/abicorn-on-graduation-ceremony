class C {
private:
  int x;
};

class C2 {
public:
  int x;
};

class C3 {
protected:
  int x;

  union {
    struct {
      int y;
    };
  };
};
