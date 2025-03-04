class A {};

template <int, int> class B {};

template <int X> class B<0, X> : A {};

class C : B<0, 1> {};

template <typename T> class D : A {};

class E : D<int> {};

// CHECK: new.cpp:5:24: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: template <int X> class B<0, X> : A {};
// CHECK-NEXT:                        ^
// CHECK-NEXT: old.cpp:5:24: note: 'B' is not derived from 'A' in the old library [inheritance-checker]
// CHECK-NEXT: template <int X> class B<0, X> {};
// CHECK-NEXT:                        ^
// CHECK-NEXT: new.cpp:5:34: note: 'B' is derived from 'A' in the new library [inheritance-checker]
// CHECK-NEXT: template <int X> class B<0, X> : A {};
// CHECK-NEXT:                                  ^
// CHECK-NEXT: new.cpp:7:11: warning: inheriting from class with modified base class hierarchy [inheritance-checker]
// CHECK-NEXT: class C : B<0, 1> {};
// CHECK-NEXT:           ^
// CHECK-NEXT: new.cpp:7:11: note: the base class hierarchy of 'B<0, 1>' has changed in the new library [inheritance-checker]
// CHECK-NEXT: new.cpp:9:29: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT:  template <typename T> class D : A {};
// CHECK-NEXT:                              ^
// CHECK-NEXT: old.cpp:9:29: note: 'D' is not derived from 'A' in the old library [inheritance-checker]
// CHECK-NEXT:  template <typename T> class D {};
// CHECK-NEXT:                              ^
// CHECK-NEXT: new.cpp:9:33: note: 'D' is derived from 'A' in the new library [inheritance-checker]
// CHECK-NEXT:  template <typename T> class D : A {};
// CHECK-NEXT:                                  ^
// CHECK-NEXT: new.cpp:11:11: warning: inheriting from class with modified base class hierarchy [inheritance-checker]
// CHECK-NEXT:  class E : D<int> {};
// CHECK-NEXT:            ^
// CHECK-NEXT: new.cpp:11:11: note: the base class hierarchy of 'D<int>' has changed in the new library [inheritance-checker]