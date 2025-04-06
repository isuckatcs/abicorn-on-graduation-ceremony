namespace foo {
class C {};
} // namespace foo

using namespace foo;

C global;

class Foo {
    C field;
    C method() { return {}; };
};

C fn(){ return {}; }

class A : C {};
