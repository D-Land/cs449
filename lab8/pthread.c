//Andrew Land

#include <pthread.h>
#include <stdio.h>

void *print_thread1();
void *print_thread2();

int main() {
  pthread_t thread1, thread2;
  int ret1;
  int ret2;

  ret1 = pthread_create(&thread1, NULL, print_thread1, NULL);
  ret2 = pthread_create(&thread2, NULL, print_thread2, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  return 0;
}

void *print_thread1(){
  while(1){
    printf("Hello from thread 1 - A \n");
    pthread_yield();
    printf("Hello from thread 1 - B \n");
  }
}

void *print_thread2(){
  while(1){
    printf("Hello from thread 2 - A \n");
    pthread_yield();
    printf("Hello from thread 2 - B \n");
  }
}
