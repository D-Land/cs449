#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int DICE[5];
int CATA[13];
int BONUS;

void roll();
int get_dice();
void reroll();
void assign_catagory();
void assign_upper(int);
void upper_bonus();
void assign_lower(int);
int of_a_kind(int);
void full_house();
void small_straight();
void large_straight();
void yahtzee();
int score();
void reset_dice();
void print_roll();
void reset_cata();
void print_cata();
int comp(const void *, const void *);

int main(){
  int i;
  srand(time(NULL));

  reset_cata();

  for(i = 0; i < 13; i++){
    reset_dice();
    roll();
    reroll();
    reroll();
    assign_catagory();
    printf("\nYour score for round %d is %d!\n", i+1, score());
  }

  return 1;
}

//Rolls any dice that are -1 in value
void roll(){
  int i;

  for(i = 0; i < 5; i++){
    if(DICE[i] == -1){
      DICE[i] = get_dice();
    }
  }
  qsort(DICE, 5, sizeof(int), comp);
  print_roll();
}

//Gets the dice from the driver
int get_dice(){
  int output;
  int i = open("dev/dice", O_RDONLY);
  int n = read(i, &output, sizeof(int));

  return output;
}

//Asks what dice the user wants to reroll
void reroll(){
  int i;
  int c;
  char input[10];


  printf("\nWhich dice would you like to reroll? Enter 0 for no reroll.\n");
  fgets(input, sizeof(input), stdin);
  strcpy(input, input);

  if(input[0] - 48 != 0){
    for(i = 0; i < strlen(input) ; i=i+2){
      if(input[i] - 49 > -1){
        DICE[input[i] - 49] = -1;
      }
    }
  }
  roll();
}

//Assign score to a user selected catagory
void assign_catagory(){
  int i = 0;
  int r;
  int not_done = 1;
  char input[1];

  while(not_done){
    printf("\nPlace dice into: \n1) Upper Section\n2) Lower Section.\n");
    scanf("%d", &i);

    if(i == 1){
      printf("\nPlace dice into: \n1) Ones \n2) Twos \n3) Threes \n4) Fours \n5) Fives \n6) Sixes\n");
      scanf("%d", &i);
      if(i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6){
        if(CATA[i-1] != -1){
          printf("\nYou have already assigned a value to this catagory. Please select another.\n");
        }
        else{
          assign_upper(i);
          not_done = 0;
        }
      }
    }
    else if(i == 2){
      printf("\nPlace dice into: \n1) Three of a kind\n2) Four of a kind\n3) Full House\n4) Small Straight\n5) Large Straight\n6) Yahtzee\n7) Chance\n");
      scanf("%d", &i);
      if(i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i ==7){
        if(CATA[i+5] != -1){
          printf("\nYou have already assigned a value to this catagory. Please select another.\n");
        }
        else{
          assign_lower(i);
          not_done = 0;
        }
      }
    }
  }
  print_cata();

  while(r != EOF && r != '\n'){
         r = getchar();
  }
}

void assign_upper(int j){
  int i;
  CATA[j-1] = 0;
  for(i = 0; i < 5; i++){
    if(DICE[i] == j){
      CATA[j-1] += 1*j;
    }
  }
  upper_bonus();
}

void upper_bonus(){
  int i;
  int total = 0;

  if(BONUS == -1){
    for(i = 0; i < 6; i++){
      if(CATA[i] != -1){
        total += CATA[i];
      }
    }
    if(total > 62){
      BONUS = 35;
    }
  }
}

void assign_lower(int j){
  switch(j){
    case 1:
      CATA[6] = 0;
      if(of_a_kind(3)){
        CATA[6] = DICE[0] + DICE[1] + DICE[2] + DICE[3] + DICE[4];
      }
      break;

    case 2:
      CATA[7] = 0;
      if(of_a_kind(4)){
        CATA[7] = DICE[0] + DICE[1] + DICE[2] + DICE[3] + DICE[4];
      }
      break;
    case 3:
      full_house();
      break;
    case 4:
      small_straight();
      break;
    case 5:
      large_straight();
      break;
    case 6:
      yahtzee();
      break;
    case 7:
      CATA[12] = DICE[0] + DICE[1] + DICE[2] + DICE[3] + DICE[4];
      break;
  }
}

int of_a_kind(int n){
  int i = 0;
  int has_n = 0;
  int current_num = DICE[0];

  for(i = 0; i < 5; i++){
    if(DICE[i] == current_num){
      has_n++;
    }
    else{
      current_num = DICE[i];
      has_n = 1;
    }
    if(has_n == n){
      return 1;
    }
  }
  return 0;
}

void full_house(){
  CATA[8] = 0;

  if(DICE[0] == DICE[1] && DICE[2] == DICE[3] && DICE[3] == DICE[4]){
    CATA[8] = 25;
  }
  else if(DICE[0] == DICE[1] && DICE[1] == DICE[2] && DICE[3] == DICE[4]){
    CATA[8] = 25;
  }
}

void small_straight(){
  CATA[9] = 0;

  if(DICE[0] < DICE[1] && DICE[1] < DICE[2] && DICE[2] < DICE[3]){
    CATA[9] = 30;
  }
  else if(DICE[1] < DICE[2] && DICE[2] < DICE[3] && DICE[3] < DICE[4]){
    CATA[9] = 30;
  }
}

void large_straight(){
  CATA[10] = 0;

  if(DICE[0] < DICE[1] && DICE[1] < DICE[2] && DICE[2] < DICE[3] && DICE[3] < DICE[4]){
    CATA[10] = 40;
  }
}

void yahtzee(){
  CATA[11] = 0;

  if(DICE[0] == DICE[1] && DICE[1] == DICE[2] && DICE[2] == DICE[3] && DICE[3] == DICE[4]){
    CATA[11] = 50;
  }
}

int score(){
  int score = 0;
  int i;

  for(i = 0; i < 13; i++){
    if(CATA[i] != -1){
      score += CATA[i];
    }
  }
  if(BONUS != -1){
    score += BONUS;
  }
  return score;
}

void reset_dice(){
  DICE[0] = -1;
  DICE[1] = -1;
  DICE[2] = -1;
  DICE[3] = -1;
  DICE[4] = -1;
}

void reset_cata(){
  BONUS = -1;
  CATA[0] = -1;
  CATA[1] = -1;
  CATA[2] = -1;
  CATA[3] = -1;
  CATA[4] = -1;
  CATA[5] = -1;
  CATA[6] = -1;
  CATA[7] = -1;
  CATA[8] = -1;
  CATA[9] = -1;
  CATA[10] = -1;
  CATA[11] = -1;
  CATA[12] = -1;
}

int comp(const void * y, const void * z){
  int a = *(const int *)y;
  int b = *(const int *)z;

  if(a==b){
    return 0;
  }
  else{
    if(a < b){
      return -1;
    }
    else{
      return 1;
    }
  }
}

void print_roll(){
  printf("\nYour Roll:");
  printf("\n%d %d %d %d %d\n", DICE[0], DICE[1], DICE[2], DICE[3], DICE[4]);
}

void print_cata(){

  printf("\nUpper Section:");
  if(BONUS == -1){
    printf("\nUpper Bonus:     has not been earned.");
  }
  else{
    printf("\nUpper Bonus: %5d", BONUS);
  }

  if(CATA[0] == -1){
    printf("\nOnes:            has not been assigned a value.");
  }
  else{
    printf("\nOnes: %12d", CATA[0]);
  }

  if(CATA[1] == -1){
    printf("\nTwos:            has not been assigned a value.");
  }
  else{
    printf("\nTwos: %12d", CATA[1]);
  }

  if(CATA[2] == -1){
    printf("\nThrees:          has not been assigned a value.");
  }
  else{
    printf("\nThrees: %10d", CATA[2]);
  }

  if(CATA[3] == -1){
    printf("\nFours:           has not been assigned a value.");
  }
  else{
    printf("\nFours: %11d", CATA[3]);
  }

  if(CATA[4] == -1){
    printf("\nFives:           has not been assigned a value.");
  }
  else{
    printf("\nFives: %11d", CATA[4]);
  }

  if(CATA[5] == -1){
    printf("\nSixes:           has not been assigned a value.");
  }
  else{
    printf("\nSixes: %11d", CATA[5]);
  }

  printf("\n\nLower Section: ");

  if(CATA[6] == -1){
    printf("\nThree of a kind: has not been assigned a value.");
  }
  else{
    printf("\nThree of a kind: %1d", CATA[6]);
  }

  if(CATA[7] == -1){
    printf("\nFour of a kind:  has not been assigned a value.");
  }
  else{
    printf("\nFour of a kind: %2d", CATA[7]);
  }

  if(CATA[8] == -1){
    printf("\nFull House:      has not been assigned a value.");
  }
  else{
    printf("\nFull House: %6d", CATA[8]);
  }

  if(CATA[9] == -1){
    printf("\nSmall Straight:  has not been assigned a value.");
  }
  else{
    printf("\nSmall Straight: %2d", CATA[9]);
  }

  if(CATA[10] == -1){
    printf("\nLarge Straight:  has not been assigned a value.");
  }
  else{
    printf("\nLarge Straight: %2d", CATA[10]);
  }

  if(CATA[11] == -1){
    printf("\nYahtzee:         has not been assigned a value.");
  }
  else{
    printf("\nYahtzee: %9d", CATA[11]);
  }

  if(CATA[12] == -1){
    printf("\nChance:          has not been assigned a value.");
  }
  else{
    printf("\nChance: %10d", CATA[12]);
  }
  printf("\n");
}
