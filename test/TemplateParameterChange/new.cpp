template <typename T, typename X, typename A> struct C { 
    C(){}; 
};
// CHECK: old.cpp:3:10: warning: method 'foo' not found in the new library [method-checker]
// CHECK-NEXT:    void foo(){};
// CHECK-NEXT:         ^

template <typename> struct D {};
// CHECK-NEXT: old.cpp:7:10: warning: method 'foo' not found in the new library [method-checker]
// CHECK-NEXT:    void foo(){};
// CHECK-NEXT:         ^
// CHECK-NOT: {{.*}}
