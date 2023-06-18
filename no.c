#include <stdio.h>

int main()
{
    char c;
    printf("(y) or (n)?: n\n");
    //scanf("%c", &c);
    c = 'n';
    if (c == 'y')
        return 0;
    else    
        return 1;

    
}