namespace {
class A {};

class B {
  class A {};
};
} // namespace

// CHECK-NOT: *
