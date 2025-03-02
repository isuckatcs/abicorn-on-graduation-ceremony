#define MACRO A
#define MACRO_FN(X) X

class A {};

class B : public MACRO {};

class C : protected MACRO_FN(A) {};
