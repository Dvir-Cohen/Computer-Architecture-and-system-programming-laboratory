#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct fun_desc {
  char *name;
  char (*fun)(char);
};

char carray[5];

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
char encrypt(char c){ /* Gets a char c and returns its encrypted form by adding 3 to its value. 
                          If c is not between 0x20 and 0x7E it is returned unchanged */
    if(c >= ' ' && c<= '~')
        c= (int)c+3;
       
    return c;
}
char decrypt(char c){ /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
    if(c >= ' ' && c<= '~')
        c= (int)c-3;
       // printf('%c\n',c);
    return c;
}
char dprt(char c){ /* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
    printf("%d\n",c);
    return c;
}
char cprt(char c){ /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
    if(c >= ' ' && c<= '~')
        printf("%c\n",c);
    else
        printf(".\n");
    return c;
}
char my_get(char c){ /* Ignores c, reads and returns a character from stdin using fgetc. */
    return fgetc(stdin);
}

char quit(char c){ /* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
    if( c == 'q'){
        exit(0);
    }
    else
        return c;
}

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
 
  for(int i= 0; i< array_length; i++){
      mapped_array[i]= (*f)(array[i]);
  }
  return mapped_array;
}
 
int main(int argc, char **argv){
  /* TODO: Test your code */
char input[1024];

char* carray2 =carray;
//char carray[5]={""};
struct fun_desc menu[] = { { "0) Censor", censor }, { "1) Encrypt", encrypt },{"2) Decrypt", decrypt}, {"3) Print dec",dprt }, {"4) Print string",cprt}, {"5) Get string",my_get }, {"6) Quit",quit} ,{ NULL, NULL } };
int i=0,option;
int bound =0;
//printf("%d\n",bound);
while(i==0){
    printf("Please choose a function:\n");
    while(menu[i].name!= NULL){
        printf("%s\n",menu[i].name);
        i++;
        bound++;
    }
    bound--;
    i=0;
    printf("Option: ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",input);
   
    option= atoi(input);
    if(option >= 0 && option <= bound){
      printf("Within bounds\n"); 
      carray2 = map(carray2,sizeof(carray2), menu[option].fun);
      printf("DONE\n\n");
      bound=0;
    }
    else{
      printf("Not within bounds\n");
      //free(carray);
      quit('q');
    }
  
}

}