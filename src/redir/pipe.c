
#include "minishell.h"

int minipipe(t_shell *shell)
{
    pid_t pid;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        ft_close(pipefd[0]);
        ft_close(pipefd[1]);
        return (-1);
    }

    if (pid == 0)
    {
        ft_close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2 (child)");
            exit(EXIT_FAILURE);
        }
        ft_close(pipefd[0]);
        shell->pipe_in = STDIN_FILENO;
        shell->status.no_exec = 0;
        shell->is_parent_process = false;
        return (2);
    }
    else 
    {
        ft_close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2 (parent)");
            ft_close(pipefd[1]);
            return (-1);
        }
        ft_close(pipefd[1]);
        shell->pipe_out = STDOUT_FILENO;
        return (1);
    }
}
