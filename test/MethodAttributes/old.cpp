class c {
public:
  int x() const;
  int x2() volatile { return 1; }

private:
  inline int y() { return 1; }

protected:
  static float z() { return 2.f; }

public:
  void leak() { y(); }
};
