#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  FILE *file;
  char string[1000];
  char input;
  int count = 0;

  if(argc != 2){
    printf("Usage: %s filename", argv[0]);
    return 1;
  }

  file = fopen(argv[1], "r");
  if(!file){
    printf("\n%s cannot be opened.\n", argv[1]);
    return 1;
  }

  do{
    input = getc(file);
    if(input > 31 && input < 127){
      string[count] = input;
      string[count + 1] = NULL;
      count++;
    }
    else{
      if(count > 3){
        printf("%s\n", string);
      }
      string[0] = NULL;
      count = 0;
    }
  }while(feof(file) == 0);

  fclose(file);
  return 0;
}
