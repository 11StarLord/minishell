#include "minishell.h"

int main(int argc,char **argv, char **env)
{
    t_shell shell;
    (void)argc;
    (void)argv;
    init_shell(&shell,env);
    return(shell.status.last_return);
}

