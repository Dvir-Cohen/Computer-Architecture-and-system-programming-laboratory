#include <stdio.h>
#define	MAX_LEN 32
extern void assFunc(int x, int y);


int main(int argc, char** argv)
{
  int input[MAX_LEN];
  int x,y;
  //printf("Please enter one number:");
  fgets(input,MAX_LEN,stdin);
  sscanf(input,"%d",input);
  x= input[0];
  //printf("Please enter another number:");
  fgets(input,MAX_LEN,stdin);
  sscanf(input,"%d",input);
  y= input[0];
  // printf("first num: %d\n", x);
  // printf("second num: %d\n", y);
  assFunc(x,y);
  return 0;
}

char c_checkValidity(int x, int y){
  if (x>=y)
    return '1';
  return '0';
}