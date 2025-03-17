#include "minishell.h"

int main(int argc,char **argv, char **env)
{
    t_shell shell;
    (void)argc;
    (void)argv;
    shell_defaults(&shell, env);
    init_shell(&shell);
    return(shell.status.last_return);
}

