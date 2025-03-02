template<typename T>
void foo(){};

template<> void foo<int>(){};
template<> void foo<double>(){};

// CHECK: old.cpp:5:17: warning: function template specialization of 'foo' not found in the new library [function-checker]
// CHECK-NEXT: template<> void foo<float>(){};
// CHECK-NEXT:                 ^
// CHECK-NEXT: new.cpp:4:17: note: found similar function [function-checker]
// CHECK-NEXT: template<> void foo<int>(){};
// CHECK-NEXT:                 ^
// CHECK-NEXT: new.cpp:5:17: note: found similar function [function-checker]
// CHECK-NEXT: template<> void foo<double>(){};
// CHECK-NEXT:                 ^
