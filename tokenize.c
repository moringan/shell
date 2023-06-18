#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "varlib.h"




/* Purpose:   to divide a string into an array of strings based on a delimeter
 * 
 * Arguments: the source, a string separated presumably containing a delimeter
 *            an array of pointers for the tokens (strings)
 *            the delimeter, as a string, eg, " " (spacebar)
 *
 * Returns:   array of pointers filled with the tokens (char **)
*/
int tokenize( char *argbuf, char **arglist, char *delim )
{
    int     word_count = 0, token_length;
    char *  token;
    if (argbuf == NULL)
    {   
        arglist = NULL;
        return 0;
    }
    token = strtok(argbuf, delim);
    
    while ((token != NULL) && (word_count < MAX_ARGS) ) {

        token_length = strlen(token) + 1;
        if ( (arglist[word_count] = malloc(token_length)) == NULL)
        {
            perror("no memory");
            exit(1); 
        }
        strcpy(arglist[word_count], token); // Copy the token into the allocated memory
        word_count++;
        
        token = strtok(NULL, delim);
       
    }
    free(token);
    arglist[word_count]=NULL;	/* close list	*/
    return word_count;
    
}
