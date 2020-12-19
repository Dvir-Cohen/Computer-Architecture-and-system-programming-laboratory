/* $Id: count-words.c 858 2010-02-21 10:26:22Z tolpin $ */
# include<stdlib.h>
#include <stdio.h>
#include <string.h>

/* return string "word" if the count is 1 or "words" otherwise */
char *words(int count) {
  char* arr = malloc(6);
  //arr= "words";
  strcpy(arr, "words");
  //arr= "words";
  //char words[] = "words";
  printf("this id the size of arr:  %u\n", sizeof(arr));
  printf(" this id the size of pointer:  %u\n", sizeof(words));
  if(count==1) {
    arr[strlen(arr)-1] = '\0';
    
}
  
  return arr;
  free(arr);
}

/* print a message reportint the number of words */
int print_word_count(char **argv) {
  int count = 0;
  char **a = argv;
  while(*(a++))
    ++count;
  printf("The sentence contains %d %s.\n", count, words(count));
  
  return count;
}

/* print the number of words in the command line and return the number as the exit code */
int main(int argc, char **argv) {
  return print_word_count(argv+1);
}

   
