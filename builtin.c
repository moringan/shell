#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "shell.h"
#include "varlib.h"

/*
 * Builtin.c
 *
 * Handle the builtin command set, and assigning a value to a variable (eg, TERM=xterm)
 * Also, substitute $vars to associated values
 * 
 * -a variable is created and assigned a value using var=val on the command line
 * -function okvarname(char *) returns true if string is A-Za-z0-9_ and does not start with 0-9
 * -all variable values are strings. if set to a number (eg X=1) then 1 is a string
 * -var/val combos are stored in an array of structs
 * -the struct type is named var, the array table[MAX_VARS] is static (available to other function in )
 * -VLstore(char *var, char *val) sets the variable X=1 or updates the variable X=2
 * -VLlookup(char *var) returns the value for var
 * -VLlist() lists table to stdout
 * -
 * -include environment variables in the list of variables
 * -users can modify and add environment variables
 */

/*
 * resultp -    a return status, not to be confused with functions whose purpose
 *              is to return TRUE or FALSE (eg. validname() )
 * 
 *  0  SUCCESS
 *  1  FAIL
 * -1  ERROR
 */

int validname(char *str)
{
    char *cp;
    for (cp = str ; *cp; cp++)
        if( (isdigit(*cp) && cp == str ) || !(isalnum(*cp) || *cp == '_' ) )
            return FALSE;
    return TRUE;
}

int builtin_command(char **arglist, int *resultp)
{
    char *val;
    int rv = FALSE;
    
    if (strcmp(arglist[0], "set") == 0)
    {
        VLlist();
        *resultp = 0;
        rv = TRUE;
    }
    else if ((val = strchr(arglist[0], '=')) != NULL)
    {
        *val = '\0';
        *resultp = validname(arglist[0]) ? VLstore(arglist[0], val+1) : -1;
        *val = '=';
        if (*resultp != -1)
            rv = TRUE;
    }
    else if (strcmp(arglist[0], "export") == 0)
    {
        if((arglist[1] != NULL) && validname(arglist[1]))
        {
            *resultp = VLexport(arglist[1]);
        }
        else
        {
            *resultp = -1;
        }
        rv = TRUE;
    }
    else if (strcmp(arglist[0], "cd" ) == 0 )
    {
        if (arglist[1])
        { 
            if (chdir(arglist[1]) < 0)
            {
                perror("cd");
            }
        }
        rv = TRUE;

    }
    
    return rv;

}


/*
 * Purpose:     substitute the variable token with it's value
 *              if there is no variable with that name, 
 *              the token is replaced with "" 
 * Argumnets:   an array of char pointers (strings)
 */
void process_special_chars(char *arglist[])
{
    int i = 0;
    char *varname;
    

    while (arglist[i] != NULL)
    {
    
        if (arglist[i][0] == '#') 
        {
            arglist[i] = NULL;
        }
        else if(arglist[i][0] == '$' && arglist[i][1] != '\0')
        {
            varname = VLlookup(&arglist[i][1]);
            if (varname[0] == '\0')
                arglist[i] = NULL;
            else
            {
                free(arglist[i]);
                arglist[i] = malloc(strlen(varname) + 1);
                strcpy(arglist[i], varname);
            }
        }

        i++;
    }

}

