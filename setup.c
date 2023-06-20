#include	<stdio.h>
#include    <stdlib.h>
#include	<signal.h>
#include    <string.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <sys/stat.h>
#include    "varlib.h"

extern char **environ;

/* Set shell to not allow control-C or control-\ 
 * so if forked processes use them it won't affect shell
 *
 * Get the environment from parent and put in our own table
*/
void setup()
{
   signal(SIGINT, SIG_IGN);
   signal(SIGQUIT, SIG_IGN);
   if ( !VLenviron2table(environ) )        
   {
       fprintf(stderr, "VLenviron2table error\n");
       exit(1);
   }
}

/* Set child back to default signals 
 *
 * Reset signals to default
 * 
 * handle <, >, and >> (connect stdin, stdout to a file )
 * 
*/ 
void setup_child(char **arglist)
{
    /*
    struct sigaction sa_default;
    sa_default.sa_handler = SIG_DFL;
    sigemptyset(&sa_default.sa_mask);
    sa_default.sa_flags = 0;
    if ( ( sigaction(SIGINT,  &sa_default, NULL) == -1 ) || 
         ( sigaction(SIGQUIT, &sa_default, NULL) == -1 ) 
       )
    {
        perror("sigaction");
        exit(1);
    }
    */
    int i = 0;
    int fd;
    //printf("setup_child()\n");

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    
    /* put our table into environ */
    if ( !VLtable2environ() )        
    {
        fprintf(stderr, "VLtable2environ error\n");
        exit(1);
    }

    /* check for redirection and change 0,1,2 file descriptors */
    for (i = 0; arglist[i] != NULL; i++)
    {
        /* send stdout (1) to a file, append */
        if ( (strcmp(arglist[i], ">>") == 0) && (arglist[i+1] != NULL) )
        {
            struct stat buffer;
            if ( (stat(arglist[i+1], &buffer) == 0) )  // check if file exists
            {   
                if (buffer.st_mode & S_IWUSR)  // check if file can be written
                {
                    // file is able to be written
                    close(1);
                    fd = open(arglist[i+1], O_WRONLY | O_APPEND);
                    if ( fd != -1)
                    {
                        arglist[i] = NULL;
                    }
                    else
                    {
                        perror("open() error");
                        arglist[0] = NULL;
                    }
                }
                else
                {
                    fprintf(stderr, "%s can't be written\n", arglist[i+1]);
                }
            }
            else
            {
                fprintf(stdout, "file %s does not exist\n", arglist[i+1]);
                arglist[0] = NULL;
            }
            break;

        }

        /* send stdout (1) to a file, create if does not exist, overwrite if exists */
        if ( (strcmp(arglist[i], ">") == 0) && (arglist[i+1] != NULL) )
        {
            
            close(1);  // close stdout file descriptor
            fd = creat(arglist[i+1], 0644);  // connect file to stdout
            arglist[i] = NULL; // the command is changed, remove > and everthing after
            break;
        }

        /* receive stdin (0) from a file */
        if ( (strcmp(arglist[i], "<") == 0) && (arglist[i+1] != NULL) )
        {
            struct stat buffer;
            if ( stat(arglist[i+1], &buffer) == 0 )
            {
                close(0);  // close stdout file descriptor
                if ( (fd = open(arglist[i+1], O_RDONLY)) != -1)
                {
                    arglist[i] = NULL;
                }
                else
                {
                    perror("open error");    
                    arglist[0] = NULL;
                }
                break;
            }
            else
            {
                fprintf(stdout, "file %s does not exist\n", arglist[i+1]);
                arglist[0] = NULL;
            }
            
            break;
        }
        
    }
    
    
}

