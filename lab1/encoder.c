#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE * output=stdout;
    FILE * input = stdin;
    char arr_const[] = {'0'+'A'-'a'};
    char* arr = arr_const;
    int debug =0, i, enc = 0, counter =0, encLength=1, lenFile;
    int sign = 1;
    for(i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0)
	        debug=1;
        if(argv[i][0] == '+' && argv[i][1] == 'e') {
            arr = argv[i]+2 ; // +e123, arr = "123"
            enc = 1;
            encLength= strlen(argv[i])-2;
        }
        if(argv[i][0] == '-' && argv[i][1] == 'e') {
            arr = argv[i]+2 ; // -e123, arr = "123"
            enc = 1;
            sign = -1;
            encLength= strlen(argv[i])-2;
        }
        if(argv[i][0] == '-' && argv[i][1] == 'o') {
            output= fopen(argv[i]+2, "w");
            lenFile= strlen(argv[i])-2;
        }
        if(argv[i][0] == '-' && argv[i][1] == 'i') {
            input= fopen(argv[i]+2, "r");
            
        }
    }

    for (i = 0; arr[i] != '\0'; i++){
        arr[i] = sign*(arr[i] - '0');
    }
    char c= fgetc(input);

    while(c!= EOF){
        if (debug ==1 && c!= '\n')
            fprintf(stderr, "%d\t", c); //%d because we want to print a the digit number
        if ((c >= 'a' && c<='z')|| (enc ==1 && c!= '\n')) // (c!= '\n') need to check this because it changes the /n char in D mode and print weird stuff in the end 
            c= c+ arr[counter];
        if (debug ==1 && c!= '\n')
            fprintf(stderr, "%d\n", c);
        if ((debug ==1|| enc==1) && c== '\n') //just for easier reading
            fprintf(stderr, "\n");
           
        if (counter == encLength-1)
            counter = 0;
        else
            counter++;
            
        fputc(c, output);
        c= fgetc(input);
    }
    if(output!=stdout)
        fclose(output);
}