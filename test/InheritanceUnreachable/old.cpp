namespace {
class A {};

class B {
  class A : ::A {};
};
} // namespace
