#include <stdlib.h>
#include <stdio.h>

/*   
 * Read input from file stream
 */


char * read_command(FILE *fp)
{   
    char c;
    int byte_count = 0;
    int current_pos = 0;
    char * string_buffer = NULL;

    printf("> ");
    while((c = getc(fp)) && (c != EOF))
    {
        
        if (byte_count == 0)
        {
            if ((string_buffer = malloc(BUFSIZ)) == NULL)
            {
                perror("no memory");
                exit(1);
            }
            byte_count += BUFSIZ;
        }
        if (current_pos > byte_count)
        {
            if ( (string_buffer = realloc(string_buffer, byte_count + BUFSIZ)) == NULL)
            {
                perror("no memory");
                exit(1);
            }
            byte_count += BUFSIZ;
        }
        if (c == '\n')
            break;
        string_buffer[current_pos++] = c; 

    }
    if (c == EOF && current_pos == 0)  // there was no data
        return NULL;
    
    
    string_buffer[current_pos] = '\0';
    
 
    return string_buffer;

}