#include <stdio.h>
#include <unistd.h>
#include "my_malloc.c"
#if 0
  printf("Debug");
#endif

void test1();
void test2();
void test3();

int main(){

  test4();

  return 0;
}

/*
 * Creates 3 Nodes
 * Frees 2nd Node
 * Frees last Node
 */
void test1(){

  int *malloc1 = my_next_fit_malloc(20);
  int *malloc2 = my_next_fit_malloc(40);
  int *malloc3 = my_next_fit_malloc(20);

  printf("\nmalloc1 %d", malloc1);
  printf("\nmalloc2 %d", malloc2);
  printf("\nmalloc3 %d\n", malloc3);

  my_malloc_print();

  my_free(malloc2);
  my_malloc_print();

  my_free(malloc3);
  my_malloc_print();
}

/*
 * Create 1 Node
 * Free Node
 */
void test2(){
  int *start;
  int *end;

  start = sbrk(0);

  int *malloc1 = my_next_fit_malloc(20);

  printf("\nmalloc1 %d\n", malloc1);
  my_malloc_print();

  my_free(malloc1);
  printf("\nFree Was Executed!\n");

  my_malloc_print();

  end = sbrk(0);

  if(start != end){
    printf("\nERROR: sbrk is not back at the starting possition.\n");
    printf("\nStart: %d\n", start);
    printf("End:   %d\n", end);
  }
}

/*
 * Create 3 Nodes
 */
void test3(){
  int *malloc1 = my_next_fit_malloc(20);
  int *malloc2 = my_next_fit_malloc(40);
  int *malloc3 = my_next_fit_malloc(20);
  int *malloc4;

  printf("\nmalloc1 %d", malloc1);
  printf("\nmalloc2 %d", malloc2);
  printf("\nmalloc3 %d", malloc3);
  my_malloc_print();

  my_free(malloc2);
  my_malloc_print();

  malloc4 = my_next_fit_malloc(10);
  printf("\nmalloc4 %d\n", malloc4);
  my_malloc_print();
}

void test4(){
  int *malloc1 = my_next_fit_malloc(20);
  int *malloc2 = my_next_fit_malloc(20);
  int *malloc3 = my_next_fit_malloc(20);
  int *malloc4 = my_next_fit_malloc(20);
  int *malloc5 = my_next_fit_malloc(20);
  int *malloc6 = my_next_fit_malloc(20);
  my_malloc_print();

  my_free(malloc2);
  my_malloc_print();

  my_free(malloc3);
  my_malloc_print();

  my_free(malloc4);
  my_malloc_print();
}
