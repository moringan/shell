#include	<stdio.h>
#include <stdlib.h>
#include	<signal.h>
#include "varlib.h"

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

/* Set child back to default signals */ 
void setup_child()
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
   signal(SIGINT, SIG_DFL);
   signal(SIGQUIT, SIG_DFL);
    
   /* get environ into our table */
   if ( !VLtable2environ() )        
   {
       fprintf(stderr, "VLtable2environ error\n");
       exit(1);
   }
}

