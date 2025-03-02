class C {
  int : 10;
  int : 10;
};
// CHECK: new.cpp:1:7: warning: change in size of record 'C' with anonymous fields [class-checker]
// CHECK-NEXT: class C {
// CHECK-NEXT:       ^
// CHECK-NEXT: old.cpp:1:7: note: record 'C' occupies 16 bits in the old library [class-checker]
// CHECK-NEXT: class C {
// CHECK-NEXT:       ^
// CHECK-NEXT: new.cpp:1:7: note: record 'C' occupies 24 bits in the new library [class-checker]
// CHECK-NEXT: class C {
// CHECK-NEXT:       ^