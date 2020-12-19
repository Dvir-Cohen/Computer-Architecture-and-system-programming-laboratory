#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void PrintHex(unsigned char *buffer, int len){
    for(int i=0; i<len;i++){       
            printf("%x  ", buffer[i]);
        }
}

int main(int argc, char **argv){
    FILE* input=fopen(argv[1], "rb");
    if(!input)
    {
        printf("error"); exit(1);
    }
    size_t size;
    fseek(input, 0, SEEK_END); 
    size = ftell(input);         
    fseek(input, 0, SEEK_SET);
    unsigned char *buffer;
    printf("%d \n", size);
    buffer = (unsigned char*) malloc(size);
    fread (buffer, sizeof(*buffer), size, input);
    printf("\n");
    PrintHex(buffer, size);
    free(buffer);
    fclose(input);    
}