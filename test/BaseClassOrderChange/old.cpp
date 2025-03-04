class A {};
class B {};
class C {};

class D : public A, public B, public C {};

struct E : public A, B {};

struct F : E {};
