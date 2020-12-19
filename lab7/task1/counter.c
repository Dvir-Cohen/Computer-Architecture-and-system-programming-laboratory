#include <stdlib.h>

#include <stdio.h>
#include <string.h>

int digit_cnt(char* argv){
    int i=0, count =0 ;
    while (argv[i]!= '\0')
    {
        if (argv[i]>='0' && argv[i]<='9')
            count ++;
        i++;
    }
    
    return count;
    
}

int main(int argc, char **argv){
    int ans;
  ans = digit_cnt(argv[1]);
  printf ("Num of digits: %d\n", ans);
}

