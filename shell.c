/*	basic linux shell by Mike Stapleton 06/2023
 *
 *  Features:
 *		Prompt shows current dir, reads command and its arguments.
 *      cd implemented
 *      Multiple commands per line with ;
 *      Run command in background with &
 *      Scripting commands - if then else fi     
 *      Shell variables - set, export, assign with = (eg TERM=xterm), recall with $
 *      execute a script from command line (shell ./myscript.sh)
 *      comment with #
 * 
 *      test first line for shebang
 * 
 * 
 */

#include	<stdio.h>
#include    <stdlib.h>
#include	<string.h>
#include    <unistd.h>
#include    <sys/wait.h>
#include    <readline/readline.h>
#include    <readline/history.h>
#include    "shell.h"


int main(int argc, char **args)
{
    char	*argbuf = (char *)NULL;  /* read line here	*/
    char    *commands[MAX_CMDS];     /* all the commands from one input line, deliniated by ;   */
    char	*arglist[MAX_ARGS];	     /* all parts of single command delimited by " "	        */
    char    *delim;                  /* string containing delimeter for strtok() (";" or " ")   */

    char    current_dir[MAX_PATH_LEN];            /* current working directory for prompt */
    
    int     return_status;  
    int     num_args;
    int     num_commands;
//    int     bg = FALSE;
    int     line = 0;

    setup();

    FILE    *fp = NULL;
    if (argc > 1)
    {
        if ( (fp = freopen(args[1], "r", stdin)) == NULL )
        {
            fprintf(stdout, "%s not found\n", args[1]);
            exit(1);
        }
        // next check the file for a shebang, by fgets, then check if first 2 chars
        // are #!   if so, put rest of string into 
        
    }

    
    while (1)
    {
        /* read current dir and set to prompt */
        getcwd(current_dir, MAX_PATH_LEN);
        int path_len = strlen(current_dir);
        current_dir[strlen(current_dir)] = '>';
        current_dir[path_len + 1] = ' ';
        current_dir[path_len + 2] = '\0';  

        /* read command from user and put into argbuf */
        if ((argbuf = readline(current_dir)) == NULL)
        {
            fprintf(stderr, "read returned NULL\n");
            break;
        }
        /* If the line has any text in it,
            save it on the history. */
        if (*argbuf) 
        {
            line++;
            add_history (argbuf);
        }

        /* users can enter more than one command per line, using the ; to deliniate */
        num_commands = tokenize(argbuf, commands, ";");
        //free(argbuf);


        /* divide the string into commands and put into a list */
        for (int i = 0; commands[i] != NULL; i++)
        {
            /* put the words of the command string into a list (arglist) */
            num_args = tokenize(commands[i], arglist, " ");
            free(commands[i]);

            /* replace $vars with values in the command, replace # with NULL */
            process_special_chars(arglist);

            /* test for #, if first char on line, test for ! then
             * save rest of line to string, that is the interpreter
             * so you then execute the interpreter, sending that
             * build a command for execvp, first token the /bin/sh
             * and second the name of the file
             * 
            */


            /* user types quit to exit shell */
            if ( (arglist[0]) && (strcmp(arglist[0], EXIT_STRING) == 0) )
                exit (0); // this isn't good, may need to do things before exiting

            /* check something is there first, then do it */
            if (arglist != NULL && argbuf[0] != '\0')
            {
                /* run the command! */
                return_status = process(arglist);
            }
            
            for(int j = 0; j < num_args; j++)
                free(arglist[j]);

        }
        free(argbuf);
        
       
    }
    
	return 0;
}

