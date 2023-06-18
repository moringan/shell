#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "varlib.h"

enum control_commands   { NONE, IF, THEN, ELSE, FI };
enum states             { NEUTRAL, WAIT_THEN, THEN_BLOCK, ELSE_BLOCK };
enum results            { SUCCESS, FAIL };

static int if_state     = NEUTRAL;
static int last_stat    = 0;
static int if_result    = SUCCESS;

/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function, if not execute()
 *  errors: arise from subroutines, handled there
 */
int process(char **arglist)
{
    int rv = 0;
    int control_command;

    
    if (arglist[0] == NULL)
        rv = 0;
    else
    {
        /* Is this a control command? */
        control_command = is_control(arglist[0]);
        
        if (control_command > 0)
            rv = do_control(arglist, control_command);
        else if (ok_to_exec(arglist))
        {   
            if (!builtin_command(arglist, &rv))
            {

                rv = execute(arglist);
            }
        }
    }
    return rv;
}


/*  
 *  Check if the command is a regular command 
 *  or a control command, return enumerated result
 */
int is_control(char *arg)
{
    if (strcmp(arg, "if") == 0)
        return IF;
    else if (strcmp(arg, "then") == 0)
        return THEN;
    else if (strcmp(arg, "else") == 0)
        return ELSE;
    else if (strcmp(arg, "fi") == 0)
        return FI;
    else 
        return NONE;
}

int syntax_error (char *msg)
{
    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", msg);
    return -1;
}


int do_control(char **arglist, int control_command)
{
    int rv = -1;
    
    switch( control_command ) 
    {
        case IF:
            printf("if detected\n");
            if (if_state != NEUTRAL)
                rv = syntax_error("if not expected");
            else
            {
             
                last_stat = process(arglist + 1);
                if_result = (last_stat == 0 ? SUCCESS : FAIL);
                
                if_state = WAIT_THEN;
                rv = 0;
            }
            break;
        case THEN:
            printf("then detected\n");
            if( if_state != WAIT_THEN )
                rv = syntax_error("then unexpected");
            else
            {
                if_state = THEN_BLOCK;
                rv = 0;
            }
            break;
        case ELSE:
            printf("else detected\n");
            if ( if_state != THEN_BLOCK)
                rv = syntax_error("else unexpected");
            else
            {
                if_state = ELSE_BLOCK;
                rv = 0;
            }
            break;
        case FI:
            printf("fi detected\n");
            if ( if_state != THEN_BLOCK && if_state != ELSE_BLOCK)
                rv = syntax_error("fi unexpected");
            else
            {
                if_state = NEUTRAL;
                rv = 0;
            }
            break;

    }

    return rv;


}

int ok_to_exec(char **arglist)
{
    int rv = 1;
    if (if_state == WAIT_THEN)
    {
        syntax_error("then expected");
        rv = 0;
    }
    else if (if_state == THEN_BLOCK && if_result == SUCCESS)
    {
        rv = 1;
    }
    else if (if_state == THEN_BLOCK && if_result == FAIL)
    {
        rv = 0;
    }
    else if ( if_state == ELSE_BLOCK && if_result == FAIL)
    {
        rv = 1;
    }
    else if ( if_state == ELSE_BLOCK && if_result == SUCCESS)
    {
        rv = 0;
    }
    
    return rv;
}
