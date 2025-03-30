struct S {
  void a() &;

  void b() &&;

  void c() &;

  void d() &;
  void d() &&;

  void e();
};

// CHECK: old.cpp:3:8: warning: overload of 'a' not found in the new library [method-checker]
// CHECK-NEXT:   void a() &&;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:2:8: note: found similar method [method-checker]
// CHECK-NEXT:   void a() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:4:8: warning: change in the ref-qualifier of method 'b' [method-checker]
// CHECK-NEXT:   void b() &&;
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp:5:8: note: method 'b' is marked with '&' ref-qualifier in the old library [method-checker]
// CHECK-NEXT:   void b() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:4:8: note: method 'b' is marked with '&&' ref-qualifier in the new library [method-checker]
// CHECK-NEXT:   void b() &&;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:6:8: warning: change in the ref-qualifier of method 'c' [method-checker]
// CHECK-NEXT:   void c() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp:7:8: note: method 'c' is marked with '&&' ref-qualifier in the old library [method-checker]
// CHECK-NEXT:   void c() &&;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:6:8: note: method 'c' is marked with '&' ref-qualifier in the new library [method-checker]
// CHECK-NEXT:   void c() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp:9:8: warning: overload of 'd' not found in the new library [method-checker]
// CHECK-NEXT:   void d();
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:8:8: note: found similar method [method-checker]
// CHECK-NEXT:   void d() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:9:8: note: found similar method [method-checker]
// CHECK-NEXT:   void d() &&;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:11:8: warning: change in the ref-qualifier of method 'e' [method-checker]
// CHECK-NEXT:   void e();
// CHECK-NEXT:        ^
// CHECK-NEXT: old.cpp:11:8: note: method 'e' is marked with '&' ref-qualifier in the old library [method-checker]
// CHECK-NEXT:   void e() &;
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:11:8: note: method 'e' is marked with no ref-qualifier in the new library [method-checker]
// CHECK-NEXT:   void e();
// CHECK-NEXT:        ^
// CHECK-NOT: {{.*}}
