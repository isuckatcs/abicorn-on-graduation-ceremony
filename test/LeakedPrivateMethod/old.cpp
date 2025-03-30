class A {
  void notLeaked() {};
  void leaked() {};

  void leaker3() { leaked(); };

protected:
  void leaker2() { leaker3(); }

public:
  void leaker() { leaker2(); };
};
