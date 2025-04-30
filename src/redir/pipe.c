
#include "minishell.h"

static int	setup_child_process(t_shell *shell, int pipe_fds[2])
{
	close(pipe_fds[1]);

	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("minishell");
		close(pipe_fds[0]);
		exit(1);
	}
	shell->pipe_in = pipe_fds[0];
	shell->status.no_exec = 0;
	shell->is_parent_process = false;
	return (2);
}

static int	setup_parent_process(t_shell *shell, int pipe_fds[2])
{
	close(pipe_fds[0]);
	if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(pipe_fds[1]);
		return (-1);
	}
	shell->pipe_out = pipe_fds[1];
	return (1);
}

int	create_pipe_process(t_shell *shell)
{
	pid_t	child_pid;
	int		pipe_fds[2];

	if (pipe(pipe_fds) == -1)
	{
		perror("minishell");
		return (-1);
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		perror("minishell");
		return (-1);
	}
	if (child_pid == 0)
		return (setup_child_process(shell, pipe_fds));
	return (setup_parent_process(shell, pipe_fds));
}

