#include "stdlib.h"
#include "stdio.h"
// clang -S -emit-llvm -O0 malloc_example.c

struct Data {
  int x;
};

int* makeInteger(int i) {
  return (int*) malloc(sizeof(int));
}

struct Data makeData1(int i) {
  struct Data toReturn;
  toReturn.x = i;
  return toReturn;
}

struct Data* makeData2(int i) {
  struct Data* toReturn;
  toReturn = (struct Data*) malloc(sizeof(struct Data));
  toReturn->x = i;
  return toReturn;
}

int main() {
  int* integer = makeInteger(5);
  struct Data data1 = makeData1(5);
  struct Data* data2 = makeData2(5);
  free(integer);
  free(data2);
  return 0;
}
