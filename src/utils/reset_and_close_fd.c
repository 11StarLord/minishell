#include "minishell.h"

void    reset_std(t_shell *shell)
{
  dup2(shell->stdin, STDIN_FILENO);
  dup2(shell->stdout, STDOUT_FILENO);
  close(shell->stdin);
  close(shell->stdout);
}

void    reset_fds(t_shell *shell)
{
    shell->fd_in = -1;
	  shell->fd_out = -1;
	  shell->pipe_in = -1;
	  shell->pipe_out = -1;
}

void  ft_close(int fd)
{
  if (fd > 0)
    close (fd);
}

void    close_fds(t_shell *shell)
{
    ft_close(shell->fd_in);
    ft_close(shell->fd_out);
    ft_close(shell->pipe_in);
    ft_close(shell->pipe_out);
}