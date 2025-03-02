template<typename T>
class c {
  T x() const;
  void y();
};

template<>
class c<int>{
  int x() const;
  void y();
};

// CHECK: old.cpp:10:8: warning: overload of 'y' not found in the new library [method-checker]
// CHECK-NEXT:   void y(int);
// CHECK-NEXT:        ^
// CHECK-NEXT: new.cpp:10:8: note: found similar method [method-checker]
// CHECK-NEXT:   void y();
// CHECK-NEXT:        ^
