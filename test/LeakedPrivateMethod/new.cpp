class A {
  void leaker3() {};

protected:
  void leaker2() { leaker3(); }

  void leakerLoop2_2() { leakerLoop2_1(); };
public:
  void leakerLoop() { leakerLoop(); };
  void leakerLoop2_1() { leakerLoop2_2(); };
  
  void leaker() { leaker2(); };
};
// CHECK: old.cpp:3:8: warning: method 'leaked' not found in the new library [method-checker]
// CHECK-NEXT:    void leaked() {};
// CHECK-NEXT:         ^
// CHECK-NOT: {{.*}}
