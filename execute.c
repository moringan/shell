#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

//static int bg = FALSE; 

int put_in_background(char **arglist)
{
    
    int num_args;

    /* get the number of strings in arglist */
    for (num_args = 0; arglist[num_args] != NULL; num_args++);
     
    /* get the size of the last string in arglist */
    int string_len = strlen(arglist[num_args-1]);

    /*
    * Check for & symbol at end of command
    * it could be the last string, or attached to end of last string
    */
    if (strcmp(arglist[num_args-1], "&") == 0)  
    {
        arglist[num_args-1]=NULL;	/* close list	*/  
        return TRUE;                /* return TRUE (indicates to run command in background) */
    }
    if (arglist[num_args-1][string_len-1] == '&')
    {
        arglist[num_args-1][string_len-1] = '\0';
        return TRUE;
    }
    return FALSE;
}

int execute( char *arglist[] )
/*
 *	use execvp to do it
 */
{
    int child_pid;
    int returned_child_id;
    int bg = FALSE; 

    int child_return_status = 0;
    
    /* check if user indicates to run in background of shell */
    if (put_in_background(arglist))
        bg = TRUE;
    
    child_pid = fork();

    
    switch(child_pid)
    {
        case -1:
            perror("fork error");
            exit(1);
        case  0:
            setup_child(arglist);
            /*for (int i = 0; arglist[i] != NULL; i++)
            {
                printf("arglist[%d] = %s\n", i, arglist[i] );
            }*/
            
            execvp(arglist[0], arglist);
            perror("execvp ");
            exit(1);
        default:
            if (bg == FALSE) /* the process is not run in background, so we do call wait() */
            {
                /* next time we come around, (next process) we'll get the return status of the bg process*/
                while((returned_child_id = wait(&child_return_status)) != child_pid)
                {
                    /* A background process */
                    if (WIFEXITED(child_return_status))
                        fprintf(stdout, "bg child %d returned with status %d\n\n", 
                        returned_child_id, WEXITSTATUS(child_return_status));
                }
                if (WIFEXITED(child_return_status))
                    fprintf(stdout, "child %d returned with status %d\n\n", 
                        returned_child_id, WEXITSTATUS(child_return_status));
                
            }
            
    }
    return child_return_status;
}

