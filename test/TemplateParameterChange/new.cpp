template <typename T, typename X, typename A> class C { 
    C(){}; 
};
// CHECK: old.cpp:3:10: warning: method 'foo' not found in the new library [method-checker]
// CHECK-NEXT:    void foo(){};
// CHECK-NEXT:         ^

template <typename> class D {};
// CHECK-NEXT: old.cpp:7:10: warning: method 'foo' not found in the new library [method-checker]
// CHECK-NEXT:    void foo(){};
// CHECK-NEXT:         ^
