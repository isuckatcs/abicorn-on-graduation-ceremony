template <typename T, typename U, typename X = int> struct C {
    C(){};
    void foo(){};
};

template <typename X = int> struct D {
    void foo(){};
};
