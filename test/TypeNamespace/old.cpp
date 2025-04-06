namespace foo {
class C {};
} // namespace foo

foo::C global;

class Foo {
    foo::C field;
    foo::C method() { return {}; };
};

foo::C fn(){ return {}; }

class A : foo::C {};
