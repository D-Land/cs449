#include <stdio.h>
#include <string.h>

int main(){
  int computer_choice = 0;
  int human_choice = 0;
  int computer_wins = 0;
  int human_wins = 0;
  char input[50];

  srand((unsigned int)time(NULL));

  printf("Welcome to Rock, Paper, Scissors\n");
  do{
    printf("Would you like to play?(yes/no)\n");
    scanf("%s", &input);

    if(input[0] == 'Y' || input[0] == 'y'){
      printf("What is your choice? (rock/paper/scissors)\n");
      scanf("%s", &input);

      if(input[0] == 'r'){
        human_choice = 0;
      }
      else if(input[0] == 'p'){
        human_choice = 1;
      }
      else if(input[0] == 's'){
        human_choice = 2;
      }

      //random number between 0 and 2
      computer_choice = rand() % 3;

      if(computer_choice != human_choice){
        if(computer_choice == 0 && human_choice == 1){
         printf("Paper covers rock. You win this round.\n");
         human_wins++;
        }
        else if(computer_choice == 0 && human_choice == 2){
         printf("Rock breaks scissors. You lost this round.\n");
         computer_wins++;
        }
        else if(computer_choice == 1 && human_choice == 0){
         printf("Paper covers rock. You lost this round.\n");
         computer_wins++;
        }
        else if(computer_choice == 1 && human_choice == 2){
         printf("Scissors cuts paper. You win this round.\n");
         human_wins++;
        }
        else if(computer_choice == 2 && human_choice == 0){
         printf("Rock breaks scissors. You win this round.\n");
         human_wins++;
        }
        else if(computer_choice == 2 && human_choice == 1){
         printf("Scissors cuts paper. You lost this round.\n");
         human_wins++;
        }
      }
      else{
        printf("It was a draw.\n");
      }
    }
    else{
      break;
    }
    printf("\nThe score is you: %d ,Computer: %d .\n", human_wins, computer_wins);
  }while(human_wins < 3 && computer_wins < 3);

  printf("\nYou won %d games. Computer won %d games.\n", human_wins, computer_wins);
  printf("\nCome play again soon!\n");

  return 0;
}
