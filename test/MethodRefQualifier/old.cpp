struct S {
  void a() &;
  void a() &&;

  void b() &;

  void c() &&;

  void d();

  void e() &;
};
