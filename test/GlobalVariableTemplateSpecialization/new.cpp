template<typename T>
int X;

template int X<int>;
template int X<double>;

// CHECK: warning: specialization of variable 'X' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int X;
// CHECK-NEXT:     ^
