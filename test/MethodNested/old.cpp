class c {
  int x() { return 0; }
  
  struct nested{
    static int x() { return 0; }
    int x(int a) { return 0; }
  };
};
