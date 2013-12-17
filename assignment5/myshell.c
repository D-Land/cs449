#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int get_num_of_args(char*);

char *DELIMITERS = " ()<>|&;\n\t";

int main(){
  char** args_array;
  char* arg;
  char* full_path;
  char* cd_path;
  char input[1024];
  char token[1024];
  int pid = 0;
  int num_of_args = 0;
  int status;
  int count = 0;

  while(1){
    printf("> ");
    fgets(input, 1024, stdin);

    num_of_args = get_num_of_args(input);
    args_array = malloc(sizeof(char*) * (num_of_args + 1));

    strcpy(token, input);

    arg = strtok(token, DELIMITERS);
    count = 0;
    while(arg != NULL){
      args_array[count] = (char*) malloc(sizeof(char) * strlen(arg));
      strcpy(args_array[count], arg);

      arg = strtok(NULL, DELIMITERS);
      count++;
    }
    args_array[num_of_args] = NULL;

    if(args_array[0] == NULL){
      continue;
    }

    if(strcmp(args_array[0], "exit") == 0){
      exit(0);
    }

    else if(strcmp(args_array[0], "cd") == 0){
      if(chdir(args_array[1]) == -1){
        if(args_array[1] == NULL){
          printf("cd: : Please specifiy a directory\n");
        }
        else{
          printf("cd: %s: No such file or director\n", args_array[1]);
        }
      }
    }

    else{

      pid = fork();

      if(pid != 0){
        wait(&status);
      }
      else{
        if(strstr(input, ">>") != NULL){
          freopen(args_array[num_of_args-1], "a", stdout);

          free(args_array[num_of_args-1]);
          args_array[num_of_args-1] = NULL;
        }

        else if(strstr(input, ">") != NULL){
          freopen(args_array[num_of_args-1], "w", stdout);

          free(args_array[num_of_args-1]);
          args_array[num_of_args-1] = NULL;
        }

        else if(strstr(input, "<") != NULL){
          freopen(args_array[num_of_args-1], "r", stdin);

          free(args_array[num_of_args-1]);
          args_array[num_of_args-1] = NULL;
        }

        execvp(args_array[0], args_array);
        exit(0);
      }
    }
  }
  return 0;
}

int get_num_of_args(char* input){
  char arguments[1024];
  char* curr;
  int count = 0;

  strcpy(arguments, input);

  curr = strtok(arguments, DELIMITERS);
  while(curr != NULL){
    count = count + 1;
    curr = strtok(NULL, DELIMITERS);
  }
  return count;
}
