#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
  int grade;
  struct Node *next;
};

main(){
  struct Node *root;
  struct Node *curr;
  int input;
  int sum = 0;
  int num_of_nodes = 0;

  root = malloc(sizeof(struct Node));

  curr = root;

  while(input != -1){
    printf("Enter a number. Enter -1 to stop.\n");
    scanf("%d", &input);
    if(input != -1){
      if(num_of_nodes != 0){
        curr->next = malloc(sizeof(struct Node));
        curr = curr->next;
      }
        curr->grade = input;
        curr->next = NULL;
        num_of_nodes++;
    }
  }

  curr = root;

  while(curr != NULL){
    sum += curr->grade;
    curr = curr->next;
  }

  while(root != NULL){
    curr = root;
    root = curr->next;
    free(curr);
  }

  printf("\nAverage: %.2f\n\n", (double)sum/num_of_nodes);
  printf("balls);

  return 0;
}
