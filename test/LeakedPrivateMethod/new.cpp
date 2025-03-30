class A {
  void leaker3() {};

protected:
  void leaker2() { leaker3(); }

public:
  void leaker() { leaker2(); };
};
// CHECK: old.cpp:3:8: warning: method 'leaked' not found in the new library [method-checker]
// CHECK-NEXT:    void leaked() {};
// CHECK-NEXT:         ^
// CHECK-NOT: {{.*}}
