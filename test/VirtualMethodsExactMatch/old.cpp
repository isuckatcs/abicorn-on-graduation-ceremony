struct S {
  virtual void foo() &;
  virtual void foo() &&;

  virtual void bar();
  virtual void bar() const;

  virtual void baz();
  virtual void baz() const;
};