#include <stdio.h>
#include <unistd.h>

typedef struct node *Node;

struct node{
  int size;
  int status;
  struct node *next;
  struct node *prev;
};

static Node first=0;
static Node last=0;
static Node curr=0;

static Node setup_first(int size){

  first = sbrk(size + sizeof(struct node));

  first->size = size;
  first->status = 0;
  first->next = NULL;
  first->prev = NULL;

  last=first;
  curr=first;

  return first;
}

static Node grow_heap(int size){
  Node n;

  n = sbrk(size + sizeof(struct node));

  n->size = size;
  n->status = 0;
  n->next = NULL;
  n->prev = last;

  last->next = n;
  last = n;

  return n;
}

static Node split_node(int size){
  Node n;

  curr->size = curr->size - (size + sizeof(struct node));

  n = (curr + sizeof(struct node)) + curr->size;

  n->size = size;
  n->status = 0;
  n->next = curr->next;
  n->prev = curr;

  curr->next->prev = n;
  curr->next = n;

  return n;
}

static Node my_next_fit(int size){
  Node n = curr->next;
  while(n != NULL){
    if(n->status == 1 && n->size == size + sizeof(struct node)){
      n->status = 0;
      n->size = size;
      curr = n;
      return n;
    }

    if(n->status == 1 && n->size > (size + sizeof(struct node))){
      curr = n;
      return split_node(size);
    }

    if(n->next == NULL){
      if(n == first){
        n = NULL;
      }
      else{
        n = first;
      }
    }
    else{
      if(n == curr){
        n = NULL;
      }
      else{
        n = n->next;
      }
    }
  }

  return (void*)(grow_heap(size) + 1);
}

void *my_next_fit_malloc(int size){
  /*if(size % 4){
    size = size + (4 - (size % 4));
  }*/

  if(first == 0){
    return (void*)(setup_first(size)+1);
  }

  return (void*)(my_next_fit(size));
}

void my_free(void *ptr){
  Node n = (Node)ptr-1;
  n->status = 1;

  if(n->prev != NULL){
    if(n->prev->status == 1){
      n->prev->size += n->size + sizeof(struct node);
      n->prev->next = n->next;
      if(n == last){
        last = n->prev;
      }
      if(n == curr){
        curr = n->prev;
      }
      n = n->prev;
    }
  }

  if(n->next != NULL){
    if(n->next->status == 1){
      n->size += n->next->size + sizeof(struct node);
      n->next = n->next->next;
    }
  }

  if(n == last){
    if(last == first){
      first = 0;
      curr = 0;
      last = 0;
      brk((void*)(n));
    }
    else if(last == curr){
      curr = curr->prev;
      last = last->prev;
      last->next = NULL;
      brk((void*)(n-1));
    }
    else{
      last = last->prev;
      last->next = NULL;
      brk((void*)(n-1));
    }
  }
}

void my_malloc_print(){
  Node n = first;
  int i = 0;
  if(first != NULL){
    while(n != NULL){
      printf("\nnode:   %d\n", i);
      printf("addr:   %d\n", n);
      printf("status: %d\n", n->status);
      printf("size:   %d\n", n->size);
      printf("next:   %d\n", n->next);
      printf("prev:   %d\n", n->prev);

      n = n->next;
      i++;
    }
  }

  else{
    printf("\n No Nodes Allocated\n");
  }
  printf("\n------------------------------------\n");
}
