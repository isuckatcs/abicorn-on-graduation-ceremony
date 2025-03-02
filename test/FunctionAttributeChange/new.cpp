void sttc() {}
void nln() {}

static void nsttc() {}
inline void nnln() {}
// CHECK: new.cpp:2:6: warning: change in the 'inline' qualifier of 'nln' [function-checker]
// CHECK-NEXT: void nln() {}
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:2:13: note: 'nln' is 'inline' in the old library [function-checker]
// CHECK-NEXT: inline void nln() {}
// CHECK-NEXT:             ^
// CHECK-NEXT: new.cpp:2:6: note: 'nln' is not 'inline' in the new library [function-checker]
// CHECK-NEXT: void nln() {}
// CHECK-NEXT:      ^
// CHECK-NEXT: old.cpp:4:6: warning: 'nsttc' is not defined in the new library [function-checker]
// CHECK-NEXT: void nsttc() {}
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:5:13: warning: change in the 'inline' qualifier of 'nnln' [function-checker]
// CHECK-NEXT: inline void nnln() {}
// CHECK-NEXT:             ^
// CHECK-NEXT: old.cpp:5:6: note: 'nnln' is not 'inline' in the old library [function-checker]
// CHECK-NEXT: void nnln() {}
// CHECK-NEXT:      ^
// CHECK-NEXT: new.cpp:5:13: note: 'nnln' is 'inline' in the new library [function-checker]
// CHECK-NEXT: inline void nnln() {}
// CHECK-NEXT:             ^
