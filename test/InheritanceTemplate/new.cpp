class A {};

template <int, int> class B : A {};

template <int X> class B<0, X> {};

class C : B<0, 1> {};

// CHECK: new.cpp:3:27: warning: change in base class hierarchy [inheritance-checker]
// CHECK-NEXT: template <int, int> class B : A {};
// CHECK-NEXT:                           ^
// CHECK-NEXT: old.cpp:3:27: note: 'B' is not derived from 'A' in the old library [inheritance-checker]
// CHECK-NEXT: template <int, int> class B {};
// CHECK-NEXT:                           ^
// CHECK-NEXT: new.cpp:3:31: note: 'B' is derived from 'A' in the new library [inheritance-checker]    
// CHECK-NEXT: template <int, int> class B : A {};
// CHECK-NEXT:                               ^
