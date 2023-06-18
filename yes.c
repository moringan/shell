#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern char **environ;

int main()
{
    char *cp = getenv("USER");
    while(*environ) {
        printf("%s\n", *environ++);
        
        }
    char *table[4];
    table[0] = "mike=1234";
    table[1] = "masdfike=1432234234";
    table[2] = "TERM=xterm";
    table[3] = NULL;

    environ = table;
    
    
    while(*environ) {
        printf("%s\n", *environ++);
        //i++;
        }
    printf("sizeof() %d strlen() %d\n", sizeof(""), strlen(""));

    
}