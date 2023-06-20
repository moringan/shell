#ifndef     SHELL_H
#define     SHELL_H

#include    <stdio.h>

#define     MAX_ARGS        100
#define     MAX_CMDS        100
#define	    ARGLEN		    1000				/* token length	*/
#define     EXIT_STRING     "quit"
#define     MAX_VAR_NAME_LEN 500
#define     MAX_PATH_LEN    500



#define     TRUE        1
#define     FALSE       0

/* function prototypes */
int     builtin_command(char **arglist, int *resultp); 
int     tokenize(char *, char **, char *);
int     execute(char *arglist[]);
int     syntax_error (char *msg);
int     ok_to_exec(char **arglist);
int     is_control(char *arg);
int     do_control(char **arglist, int control_command);
//char   *read_command(FILE *fp);
void    setup();
void    setup_child(char **);
int     put_in_background(char **);
int     process(char **arglist);
void    process_special_chars(char *arglist[]);


#endif