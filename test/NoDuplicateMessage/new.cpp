// CHECK: In file included from {{.*}}/NoDuplicateMessage/old1.cpp:1:
// CHECK-NEXT: {{.*}}/NoDuplicateMessage/old.h:1:5: warning: 'x' is not declared in the new library [global-variable-checker]
// CHECK-NEXT:     1 | int x;
// CHECK-NEXT:       |     ^
// CHECK-NOT: {{.*}}