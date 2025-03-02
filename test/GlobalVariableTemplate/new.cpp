int Template;

template<typename T>
int NonTemplate;

// CHECK: warning: 'Template' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int Template;
// CHECK-NEXT:     ^
// CHECK-NEXT: warning: 'NonTemplate' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int NonTemplate;
// CHECK-NEXT:     ^
