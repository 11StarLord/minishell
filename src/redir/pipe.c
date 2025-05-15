
#include "minishell.h"

static int	setup_child_process(t_shell *shell, int *pipe_fds)
{
	ft_close(pipe_fds[1]);
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 child");
		close(pipe_fds[0]);
		return (-1);
	}
	shell->pipe_in = pipe_fds[0];
	shell->status.no_exec = 0;
	shell->is_parent_process = false;
	return (2);
}

static int	setup_parent_process(t_shell *shell, int *pipe_fds)
{
	ft_close(pipe_fds[0]);
	if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 parent");
		close(pipe_fds[1]);
		return (-1);
	}
	shell->pipe_out = pipe_fds[1];
	shell->is_parent_process = true;
	return (1);
}

int	create_pipe_process(t_shell *shell)
{
	pid_t	pid;
	int		pipe_fds[2];

	if (pipe(pipe_fds) == -1)
	{
		perror("minishell: pipe");
		shell->status.last_return = 1;
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
		return (setup_child_process(shell, pipe_fds));
	else
		return (setup_parent_process(shell, pipe_fds));
	return (0);
}

/*void	create_pipe_process2(t_shell *shell)
{
	int	p_fd[2];
	int	pid;
	int	status;
	char	**cmd;

	status = 0;
	pipe(p_fd);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork1");
		return ;
	}
	if (pid == 0)
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[0]);
		close(p_fd[1]);
		cmd = token_to
	}
}*/