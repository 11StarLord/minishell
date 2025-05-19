#include "minishell.h"

int	minipipe(t_shell *shell)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		ft_close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		shell->pipe_in = pipefd[0];
		shell->status.no_exec = 0;
		shell->is_parent_process = false;
		return (2);
	}
	else
	{
		ft_close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		shell->pipe_out = pipefd[1];
		return (1);
	}
}