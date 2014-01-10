#include <cstdio>
// clang++ -S -emit-llvm -O0 toconvert.cpp

int I(int x) {
  return x;
}

int (*getPointerToI())(int) {
  int (*functionPointer)(int) = I;
  return functionPointer;
}

int main() {
  printf("%d\n", getPointerToI()(1));
  return 0;
}
