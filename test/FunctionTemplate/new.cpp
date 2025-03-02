void foo(){};

template<typename T>
void bar(){};

// CHECK: old.cpp:2:6: warning: function template declaration of 'foo' not found in the new library [function-checker]
// CHECK-NEXT: void foo(){};
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:4:6: warning: overload of 'bar' not found in the new library [function-checker]
// CHECK-NEXT: void bar(){};
// CHECK-NEXT:      ^
