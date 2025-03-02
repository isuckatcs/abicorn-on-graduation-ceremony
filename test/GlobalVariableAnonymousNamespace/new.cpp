namespace {
    int Reachable;
} // namespace
// CHECK: warning: 'Reachable' is not declared in the new library [global-variable-checker]
// CHECK-NEXT: int Reachable;
// CHECK-NEXT:     ^

int Unreachable;
