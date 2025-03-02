// Dtors used to crash.
// CHECK-NOT: warning

class c {
};

class d {
  ~d() = default;
};
