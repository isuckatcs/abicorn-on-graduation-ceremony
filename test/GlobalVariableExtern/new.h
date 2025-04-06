extern const int foo[];

const int foo[] = {1, 2, 3, 4, 5};

extern const float bar[];
// CHECK: new.h:[[# @LINE - 1 ]]:20: warning: change in the type of 'bar' [global-variable-checker]
// CHECK-NEXT:  extern const float bar[];
// CHECK-NEXT:                     ^
// CHECK-NEXT: old.h:5:18: note: the type of 'bar' is 'const int[]' in the old library [global-variable-checker]
// CHECK-NEXT:  extern const int bar[];
// CHECK-NEXT:                   ^
// CHECK-NEXT: new.h:[[# @LINE - 7 ]]:20: note: the type of 'bar' is 'const float[]' in the new library [global-variable-checker]
// CHECK-NEXT:  extern const float bar[];
// CHECK-NEXT:                     ^
const float bar[] = {1, 2, 3, 4, 5};
// CHECK-NOT: {{.*}}