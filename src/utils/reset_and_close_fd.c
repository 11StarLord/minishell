#include "minishell.h"

void    reset_std(t_shell *shell)
{
    dup2(0, shell->in);
    dup2(1, shell->out);
}

void    reset_fds(t_shell *shell)
{
    shell->fd_in = -1;
	shell->fd_out = -1;
	shell->pipe_in = -1;
	shell->pipe_out = -1;
}

void    close_fds(t_shell *shell)
{
    if (shell->fd_in > 0)
		close(shell->fd_in);
    if (shell->fd_out > 0)
		close(shell->fd_out);
    if (shell->pipe_in > 0)
		close(shell->pipe_in);
    if (shell->pipe_out > 0)
		close(shell->pipe_out);
}