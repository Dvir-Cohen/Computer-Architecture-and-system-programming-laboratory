#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int debugFlag= 0;
struct fun_desc {
    char *name;
    void (*fun)();
};

char* sectionToType(int num){
    char *s= "NOT VAILD";
    switch (num) {
        case SHT_NULL:
            s= "NULL";
            break;
        case SHT_PROGBITS:
            s= "PROGBITS";
            break;
        case SHT_SYMTAB:
            s= "SYMTAB";
            break;
        case SHT_STRTAB:
            s= "STRTAB";
            break;
        case SHT_RELA:
            s= "RELA";
            break;
        case SHT_HASH:
            s= "HASH";
            break;
        case SHT_DYNAMIC:
            s= "DYNAMIC";
            break;
        case SHT_NOTE:
            s= "NOTE";
            break;
        case SHT_NOBITS:
            s= "NOBITS";
            break;
        case SHT_REL:
            s= "REL";
            break;
        case SHT_SHLIB:
            s= "SHLIB";
            break;
        case SHT_DYNSYM:
            s= "DYNSYM";
            break;
        case SHT_INIT_ARRAY:
            s= "INIT_ARRAY";
            break;
        case SHT_FINI_ARRAY:
            s= "FINI_ARRAY";
            break;
        case SHT_PREINIT_ARRAY:
            s= "PREINIT_ARRAY";
            break;
        case SHT_GROUP:
            s= "GROUP";
            break;
        case SHT_SYMTAB_SHNDX:
            s= "SYMTAB_SHNDX";
            break;
        case SHT_NUM:
            s= "NUM";
            break;
        case SHT_LOOS:
            s= "LOOS";
            break;
        case SHT_GNU_ATTRIBUTES:
            s= "GNU_ATTRIBUTES";
            break;
        case SHT_GNU_HASH:
            s= "GNU_HASH";
            break;
        case SHT_GNU_LIBLIST:
            s= "GNU_LIBLIST";
            break;
        case SHT_CHECKSUM:
            s= "CHECKSUM";
            break;
        case SHT_LOSUNW:
            s= "LOSUNW";
            break;
    
        case SHT_SUNW_COMDAT:
            s= "SUNW_COMDAT";
            break;
        case SHT_SUNW_syminfo:
            s= "SUNW_syminfo";
            break;
        case SHT_GNU_verdef:
            s= "VERDEF";
            break;
        case SHT_GNU_verneed:
            s= "VERNEED";
            break;
         case SHT_GNU_versym:
             s= "VERSYM";
             break;
        case SHT_LOPROC:
            s= "LOPROC";
            break;
        case SHT_HIPROC:
            s= "HIPROC";
            break;
        case SHT_LOUSER:
            s= "LOUSER";
            break;
        case SHT_HIUSER:
            s= "HIUSER";
            break;
    }
    return s;
}
void debugMode (){
    if (debugFlag == 0){
        printf("Debug flag now on\n");
        debugFlag=1;
        return;
    }
    printf("Debug flag now off\n");
    debugFlag =0;
}
void quit(){
    if (debugFlag)
        printf("quitting\n");
    exit(0);
    }
void examine(){
    char* s;
    int fd;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */
    Elf32_Ehdr *header; /* this will point to the header structure */

    printf ("Choose an ELF file:\n");
    scanf("%s",s);
   

   if( (fd = open(s, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_PRIVATE, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }

   /* now, the file is mapped starting at map_start.
    * all we need to do is tell *header to point at the same address:
    */

    header = (Elf32_Ehdr *) map_start;
    if ((header->e_ident[0] != ELFMAG0) || (header->e_ident[1] != ELFMAG1) || (header->e_ident[2] != ELFMAG2) || (header->e_ident[3] != ELFMAG3)) {
        printf("%s-not an ELF file\n", s);
    }
    else{
        printf("*****************\n");
        printf("Magic Number: \t%.3s\n", header->e_ident+1); 
        printf("Data encoding: \t");
  
        if ((int)header->e_ident[5] == ELFDATA2LSB) 
            printf("2's complement, little endian\n");
        else // mean its ELFDATA2MSB
            printf("2's complement, big endian\n");
        printf("Entry point:\t %x\n", header->e_entry);
        printf("Section header offset: \t%d (bytes into file)\n", header->e_shoff); 
        printf("Number of section header entries: \t%d\n", header->e_shnum); 
        printf("Size of each section header entry: \t%d (bytes)\n", header->e_shentsize); 
        printf("File offset in which the program header table resides: \t%d (bytes)\n", header->e_phoff); 
        printf("Number of program header entries: \t%d\n", header->e_phnum); 
        printf("Size of each program header entry: \t%d (bytes)\n", header->e_phentsize); 
        printf("*****************\n");
    }
    close(fd);
    

   /* now, we unmap the file */
   munmap(map_start, fd_stat.st_size);
}
void PrintSection(){
   
    char* s;
    int fd, i;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */
    Elf32_Ehdr *header; /* this will point to the header structure */
    Elf32_Shdr *section; /* this will point to the section header structure */
    Elf32_Shdr *stringTable; 
    s= malloc(100);
    printf ("Choose an ELF file:\n");
    scanf("%s",s);
    // fflush(stdin);
    // fgets(s,100,stdin);
    // sscanf(s,"%s",s);

   if( (fd = open(s, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_PRIVATE, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }
    header = (Elf32_Ehdr *) map_start;
    if ((header->e_ident[0] != ELFMAG0) || (header->e_ident[1] != ELFMAG1) || (header->e_ident[2] != ELFMAG2) || (header->e_ident[3] != ELFMAG3)) {
        printf("%s-not an ELF file\n", s);
    }
    else{
        section= (Elf32_Shdr *)(header->e_shoff+ map_start);
        printf("section-  %X\n", section);
        printf("header-  %X\n", header->e_shstrndx);
        stringTable= section+ header->e_shstrndx;
        //printf("index\t section_name\t section_address  section_offset\t section_size\t  section_type\n");
        printf("%s\t %-15s %-15s %-15s %-15s %-15s\n", "index","section_name","section_address", "section_offset", "section_size", "section_type");
        printf("--------------------------------------------------------------------------------------------\n");
        for (i = 0; i < header->e_shnum; i++)
        {
            printf("[%2d]\t", i);
            printf("%-15s\t",(char*)(stringTable->sh_offset +map_start)+ section[i].sh_name);
            printf("%-15x\t", section[i].sh_addr);
            printf("%-15x\t", section[i].sh_offset);
            printf("%-15x\t", section[i].sh_size);
            printf("%-15s\n", sectionToType(section[i].sh_type));

        }
        
    }
    close(fd);
}
void PrintSymbols(){
    char* s;
    int fd, i;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */
    Elf32_Ehdr *header; /* this will point to the header structure */
    Elf32_Shdr *section; /* this will point to the section header structure */
    Elf32_Shdr *stringTable;
    char* stringOff; 
    s= malloc(100);
    printf ("Choose an ELF file:\n");
    scanf("%s",s);


   if( (fd = open(s, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_PRIVATE, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }
    header = (Elf32_Ehdr *) map_start;
    if ((header->e_ident[0] != ELFMAG0) || (header->e_ident[1] != ELFMAG1) || (header->e_ident[2] != ELFMAG2) || (header->e_ident[3] != ELFMAG3)) {
        printf("%s-not an ELF file\n", s);
    }
    else{
        section= (Elf32_Shdr *)(header->e_shoff+ map_start);
        stringTable= section+ header->e_shstrndx;
        stringOff = (char*)(stringTable->sh_offset +map_start);

        Elf32_Shdr* symbolHeader;
        char* symbolName;

        for (i = 0; i < header->e_shnum; i++) {
            if ( section[i].sh_type == SHT_SYMTAB) {
                symbolHeader = (section + i);
            }
            if (section[i].sh_type == SHT_STRTAB && (strcmp((stringOff + section[i].sh_name), ".strtab") == 0)) {
                symbolName = (char*)(map_start + section[i].sh_offset);
                break;
            }
        }
    
        Elf32_Sym* symbol = (Elf32_Sym*) (map_start + symbolHeader->sh_offset);
        if(debugFlag==1){
            printf("size of symbol table is:  %d\n", symbolHeader->sh_size);
            printf("number of sybmols therein:  %d\n", symbolHeader->sh_size / symbolHeader->sh_entsize);
        }
        printf("%s\t %-12s %-15s %-15s %-15s \n", "index","value","section_index", "section_name", "symbol_name");
        printf("------------------------------------------------------------------\n");
        
        for (i = 0; i < (symbolHeader->sh_size / symbolHeader->sh_entsize); i++) {
            printf("[%2d]\t",i);
            printf("%-10x\t", symbol[i].st_value);
            if(symbol[i].st_shndx != 65521 && symbol[i].st_shndx != 0 )
                printf("%-10d   ", symbol[i].st_shndx);
            else{
                if(symbol[i].st_shndx == 65521)
                    printf("%-10s   ","ABS");
                else
                     printf("%-10s   ","UND");
                
            }
            if(symbol[i].st_shndx != 65521 && symbol[i].st_shndx != 0 )
                printf("%-15s\t", (stringOff + section[symbol[i].st_shndx].sh_name));
            else
                printf("%-15s\t","null");
                   
            printf("%-20s\n",  (symbolName + symbol[i].st_name));
        }
    }
    close(fd);
}
void Relocation(){
    char* s;
    int fd, i,j;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */
    Elf32_Ehdr *header; /* this will point to the header structure */
    Elf32_Shdr *section; /* this will point to the section header structure */
    Elf32_Shdr *stringTable;
    char* stringOff; 
    s= malloc(100);
    printf ("Choose an ELF file:\n");
    scanf("%s",s);

   if( (fd = open(s, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_PRIVATE, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }
    header = (Elf32_Ehdr *) map_start;
    if ((header->e_ident[0] != ELFMAG0) || (header->e_ident[1] != ELFMAG1) || (header->e_ident[2] != ELFMAG2) || (header->e_ident[3] != ELFMAG3)) {
        printf("%s-not an ELF file\n", s);
    }
    else{
        section= (Elf32_Shdr *)(header->e_shoff+ map_start);
        stringTable= section+ header->e_shstrndx;
        stringOff = (char*)(stringTable->sh_offset +map_start);

        Elf32_Shdr* relHeader;
        char* symbolName;
        Elf32_Shdr* symbolHeader;
        Elf32_Sym* symbol;

         for (i = 0; i < header->e_shnum; i++) {
            if ( section[i].sh_type == SHT_DYNSYM) {
                symbolHeader = (section + i);
            }
             if (strcmp(((char*)stringOff + section[i].sh_name), ".dynsym") == 0) {
                printf("im here\n");
                symbolName = (char*)(map_start + section[section[i].sh_link].sh_offset);
            }
         }
        for (i = 0; i < header->e_shnum; i++) {
           
            
            if ( section[i].sh_type == SHT_REL){ 
                relHeader = (section + i);
                Elf32_Rel* relTable = (Elf32_Rel*) (map_start + relHeader->sh_offset);
                printf("%s  %8s %8s\t %8s\t  %8s\n", "Offset","Info", "Type", "Sym.Value","Sym. Name");
                printf("---------------------------------------------------------------------\n");
            for (j= 0; j < (relHeader->sh_size / relHeader->sh_entsize); j++)
            {
                printf("%x\t",relTable[j].r_offset);
                printf("%8x", relTable[j].r_info);
                printf("%8d\t", ELF32_R_TYPE(relTable[j].r_info));
                symbol = (Elf32_Sym*) (map_start + symbolHeader->sh_offset);
               
                printf("%8x\t", symbol[ELF32_R_SYM(relTable[j].r_info)].st_value);
                printf("%8s\n", symbolName+symbol[ELF32_R_SYM(relTable[j].r_info)].st_name);
            }
            printf("\n\n");
            }
        }
        
    }
    close(fd);
}

int main(int argc, char **argv){
    int i, input, bound;
    struct fun_desc menu[] = { {"Toggle Debug Mode",debugMode},{ "Examine ELF File", examine},{"Print Section Names", PrintSection},{"Print Symbols", PrintSymbols},{"Relocation Tables", Relocation},{"Quit", quit},{ NULL, NULL } };
    bound = (sizeof(menu))/(sizeof(menu[0])) - 1;
    while (1){
       // if (debugFlag)
         //   printf("---\nunit size: %d\nfile name: %s\nmem count:%d\n---\n",s->unit_size,s->file_name,s->mem_count);
        printf ("Choose action:\n");
        for (i=0; menu[i].name != NULL; i++) 
            printf ("%d - %s\n", i, menu[i].name);
        printf ("option: ");
        scanf("%d",&input);
        if (input>= 0 && input <= 5)
             menu[input].fun();
        else
        {
            printf("bad input\n");
            break;
        }
    }
}