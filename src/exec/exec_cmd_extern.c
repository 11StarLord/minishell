#include "minishell.h"

static char *error_message(char *path, t_shell *shell)
{
	DIR	*folder;
	int	fd;

	fd = open(path, O_WRONLY);
	folder = opendir(path);
	if (fd != -1)
		close (fd);
	if (folder)
		closedir(folder);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ft_strchr(path, '/') == NULL)
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		shell->status.last_return = 127;
		return (NULL);
	}
	perror(path);
	if (errno == EACCES)
		shell->status.last_return = 126;
	else
		shell->status.last_return = 127;
	return (NULL);
}

static void execute_child(t_shell *shell, char **cmd)
{
	char *cmd_path;
	char	**env_matrix;

	cmd_path = get_command_path(cmd[0], shell);
	if (cmd_path)
	{
		env_matrix = env_to_matrix(shell->env, 0, false);
		execve(cmd_path, cmd, env_matrix);
		free_matrix(env_matrix);
	}
	else
	{
		error_message(cmd[0], shell);
		shell->is_parent_process = false;
	}
	free(cmd_path);
}

void	execute_command(t_shell *shell, char **cmd)
{
	int		status;
	pid_t	pid;

	if (shell->charge == 0)
		return ;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	pid = fork();
	if (pid == 0)
	{
    	reset_signals_in_child();
		execute_child(shell, cmd);
		exit(shell->status.last_return);
	}
	else
	{
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);

		if (WIFEXITED(status))
			shell->status.last_return = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->status.last_return = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		}
	}

	shell->charge = 0;
}
