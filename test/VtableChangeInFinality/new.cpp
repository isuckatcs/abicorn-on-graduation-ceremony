class Class {
    virtual void foo() final{};
    //CHECK: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in virtual function finality [vtable-checker]
    //CHECK-NEXT:    virtual void foo() final{};
    //CHECK-NEXT:                  ^
    //CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: virtual function 'foo' is non-final in the old library [vtable-checker]
    //CHECK-NEXT:     virtual void foo(){};
    //CHECK-NEXT:                  ^
    //CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: virtual function 'foo' is final in the new library [vtable-checker]
    //CHECK-NEXT:     virtual void foo() final{};
    //CHECK-NEXT:                  ^

    virtual void bar() {};
    //CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} warning: change in virtual function finality [vtable-checker]
    //CHECK-NEXT:     virtual void bar() {};
    //CHECK-NEXT:                  ^
    //CHECK-NEXT: old.cpp{{:[0-9]*:[0-9]*:}} note: virtual function 'bar' is final in the old library [vtable-checker]
    //CHECK-NEXT:     virtual void bar() final{};
    //CHECK-NEXT:                  ^
    //CHECK-NEXT: new.cpp{{:[0-9]*:[0-9]*:}} note: virtual function 'bar' is non-final in the new library [vtable-checker]
    //CHECK-NEXT:     virtual void bar() {};
    //CHECK-NEXT:                  ^
};
