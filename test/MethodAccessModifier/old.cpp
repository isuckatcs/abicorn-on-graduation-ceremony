class c {
public:
  int x() { return 0; };

private:
  int y() { return 1; };

protected:
  float z() { return 2.f; }

public:
  void leak() { y(); }
};
