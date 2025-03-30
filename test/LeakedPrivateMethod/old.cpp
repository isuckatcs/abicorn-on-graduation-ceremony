class A {
  void notLeaked() {};
  void leaked() {};

  void leaker3() { leaked(); };

protected:
  void leaker2() { leaker3(); }

  void leakerLoop2_2() { leakerLoop2_1(); };
public:
  void leakerLoop() { leakerLoop(); };
  void leakerLoop2_1() { leakerLoop2_2(); };

  void leaker() { leaker2(); };
};
