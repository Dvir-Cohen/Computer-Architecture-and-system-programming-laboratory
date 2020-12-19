#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

virus* v ;

typedef struct link link; 
struct link {
    link *nextVirus;
    virus *vir;
};
struct fun_desc {
  char *name;
};
void PrintHex(unsigned char *buffer, int len){
    for(int i=0; i<len;i++){       
            printf("%02X ", buffer[i]);
        }
}

virus* readVirus(FILE* f){
    if(!f)
    {
        printf("error"); exit(1);
    }
    v = malloc(sizeof(virus));
    unsigned char *buffer;
    buffer = (unsigned char*) calloc(18,1);
    fread(buffer, 1, 18, f);
    
    v->SigSize=(unsigned short) (buffer[0]+ buffer[1]*256);
    for (int i = 0; i < 16; i++)
      v->virusName[i]= buffer[i+2];
    free(buffer);
    v->sig = (unsigned char*) malloc(v->SigSize);
    if (fread( v->sig , 1, v->SigSize, f)!= v->SigSize)
    {
       free(v);
       return NULL;
    }
    return v;
    free (v->sig);
    free (v);
     
}

void printVirus(virus* virus, FILE* output){
    fprintf(output,"VIRUS NAME: %s\n",virus->virusName);
    fprintf(output,"VIRUS size: %d\n",virus->SigSize);
    printf("VIRUS sig:\n");
    PrintHex(virus->sig,virus->SigSize);
    free(virus);
}

link* list_append(link* virus_list, virus* data){
     /* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */

    link *newVirus = calloc(sizeof(link),1);
   // if (virus_list->vir != NULL)
    if (virus_list != NULL)
    {
        newVirus->nextVirus= virus_list;
    }
    else
    {
        newVirus->nextVirus= NULL;
    }
    
    newVirus->vir= data;
    return newVirus;
    free(newVirus);
}
 void list_print(link *virus_list, FILE* f){ 
     /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
    
    while (virus_list != NULL){
       if (virus_list->vir!=NULL){
            printVirus(virus_list->vir, f);
            printf("\n");
    }
       // free(virus_list->vir);
        virus_list= virus_list->nextVirus;
          
    }
    //free(virus_list);
}
 

void list_free(link *virus_list){ /* Free the memory allocated by the list. */
    printf("clean up\n");
    while (virus_list!= NULL)
    {
        free(virus_list->vir);
        //virus_list->vir= virus_list->nextVirus->vir;
        virus_list= virus_list->nextVirus;
    }
    free(virus_list);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){
    
    while (virus_list->vir!= NULL)
    {   int i= 0;
    char *sig= "I am virus1!";
        while (i<size)
        {   if(memcmp((buffer+i),sig, virus_list->vir->SigSize)==0){
                // (memcmp((buffer+i+2),virus_list->vir->virusName, 16)==0)&&
                // (memcmp((buffer+i+18),virus_list->vir->sig, virus_list->vir->SigSize)==0)){
                printf("Starting byte:  %d\n", i);
                printf("Virus name: %s\n", virus_list->vir->virusName);
                printf("Virus size: %d\n", virus_list->vir->SigSize);
                printf("\n");
                break;
            }
                i++;
                //printf("i:  %d\n", i);
        }
        virus_list=virus_list->nextVirus;
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
    // virus* v = malloc(sizeof(virus));
    // v= readVirus(input);
    // while (v!=NULL)
    // {
    //    printVirus(v, stdout);
    //     printf("\n");
    //     free (v->sig);
    //      free(v);
    //    v= readVirus(input);
    // }
    // //free (v->sig);
    // free(v);
    // 
   
    unsigned int smaller;
    //char buffer[10000];
    unsigned char *buffer;
    buffer = (unsigned char*) calloc(10000,1);
    fread (buffer, sizeof(*buffer), 10000, input);
    fseek(input, 0, SEEK_SET);
    struct fun_desc menu[] = { { "1) Load signatures"}, { "2) Print signatures" },{"3) Detect viruses"},{"4) Quit"} ,{ NULL } };
    int i=0,option;
    link *linklist= calloc(sizeof(link), 1);
     linklist->vir=NULL;
    linklist->nextVirus=NULL;
    char screen[1024];
    while(i==0){
    printf("Please choose a function:\n");
    while(menu[i].name!= NULL){
        printf("%s\n",menu[i].name);
        i++;
    }
     i=0;
    printf("Option: ");
    fgets(screen,sizeof(screen),stdin);
    sscanf(screen,"%s",screen);
   
    option= atoi(screen);
    switch (option)
    {
    case 1:
        linklist= list_append(linklist, readVirus(input));
        break;
    case 2: 
        list_print(linklist, stdout);
        linklist->vir=NULL;
        linklist->nextVirus=NULL;
        break;
    case 3:
        
       // PrintHex(buffer,size);
        if ( 10000< size)
            smaller= 10000;
        else
        {
            smaller= size;
        }
        detect_virus(buffer, smaller ,linklist);
        break;
    case 4:
        list_free(linklist);
        i=1;
    default:
        break;
    }

     
}
free(linklist->nextVirus);
free(linklist->vir);
free(linklist);
fclose(input);
return 0;
}