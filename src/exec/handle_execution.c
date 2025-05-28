#include "minishell.h"

static char	*append_value(char *base, char *value, bool has_quote)
{
	char	*tmp;

	if (has_quote)
	{
		tmp = ft_strjoin(base, "\"");
		ft_free(base);
		base = ft_strjoin(tmp, value);
		ft_free(tmp);
		tmp = ft_strjoin(base, "\"");
		ft_free(base);
		base = tmp;
	}
	else
	{
		tmp = ft_strjoin(base, value);
		ft_free(base);
		base = tmp;
	}
	return (base);
}

static char *append_key_equals(char *key)
{
	char	*result;

	result = ft_strjoin(key, "=");
	return (result);
}

static char *format_env_value(char *key, char *value, int equal, bool has_quote)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strdup(key);
	if (!tmp)
		return (NULL);
	if (value && equal == 1)
	{
		joined = append_key_equals(tmp);
		ft_free(tmp);
		tmp = append_value(joined, value, has_quote == true);
	}
	return (tmp);
}

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
	perror(path);
	if (ft_strchr(path, '/') == NULL)
		shell->status.last_return = 127;
	else if (errno == EACCES)
		shell->status.last_return = 126;
	else
		shell->status.last_return = 127;
	return (NULL);
}

static char *join_path_cmd(const char *path, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	ft_free(tmp);
	return (full);
}

static char *search_path(const char *cmd, char **path_dirs)
{
	char	*full_path;
	int	i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = join_path_cmd(path_dirs[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		ft_free(full_path);
		i++;
	}
	return (NULL);
}

static char *check_access_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

static char *get_command_path(char *cmd, t_shell *shell)
{
	char	**path_dirs;
	char	*result;

	result = check_access_path(cmd);
	if (result)
		return (result);
	shell->tmp.str = ft_getenv(shell->env, "PATH");
	if (!shell->tmp.str)
		return (NULL);
	path_dirs = ft_split(shell->tmp.str, ':');
	ft_free(shell->tmp.str);
	if (!path_dirs)
		return (NULL);
	result = search_path(cmd, path_dirs);
	free_matrix(path_dirs);
	return (result);
}

static int	get_env_count(t_env *env)
{
	int	count;
	t_env	*env_tmp;

	env_tmp = env;
	while (env_tmp)
	{
		count++;
		env_tmp = env_tmp->next;
	}
	return (count);
}

static char **env_to_matrix(t_env *env, int i, bool has_quote)
{
	t_env	*env_tmp;
	char	**matrix;

	env_tmp = env;
	i = get_env_count(env);
	matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (!matrix)
		return (NULL);
	i = 0;
	while (env_tmp)
	{
		if (has_quote == true && ft_strncmp(env_tmp->key, "_", 1) == 0)
		{
			env_tmp = env_tmp->next;
			continue ;
		}
		matrix[i] = format_env_value(env_tmp->key, env_tmp->value, env_tmp->equal, has_quote);
		if (matrix[i])
			i++;
		env_tmp = env_tmp->next;
	}
	matrix[i] = NULL;
	return (matrix);
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

static void	execute_command(t_shell *shell, char **cmd)
{
	int	status;
	pid_t	pid;

	status = 0;
	if (shell->charge == 0)
		return ;
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		execute_child(shell, cmd);
		exit(shell->status.last_return);
	}
	else
	{
		waitpid(pid, &status, 0);
		signal(SIGQUIT, SIG_DFL);
	}
	shell->charge = 0;
	shell->status.last_return = WEXITSTATUS(status);
}

static	char **tokens_to_cmd(t_token *token, int *pos_token)
{
	char	**cmd;
	int	i;
	int	j;

	if (!token)
		return (NULL);
	i = *pos_token;
	j = 0;
	while (token[i].str && is_type_token(token[i], "ARGUMENT"))
	{
		i++;
		j++;
	}
	cmd = malloc(sizeof(char *) * (j + 1));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(char *) * (j + 1));
	i = 0;
	while (token[*pos_token].str && is_type_token(token[*pos_token], "ARGUMENT"))
	{
		cmd[i++] = ft_strdup(token[*pos_token].str);
		(*pos_token)++;
	}
	cmd[i] = NULL;
	return (cmd);
}

int	is_builtin(char **cmd)
{
	return (!ft_strcmp(cmd[0], "echo")
		|| !ft_strcmp(cmd[0], "cd")
		|| !ft_strcmp(cmd[0], "pwd")
		|| !ft_strcmp(cmd[0], "env")
		|| !ft_strcmp(cmd[0], "export")
		|| !ft_strcmp(cmd[0], "unset")
		|| !ft_strcmp(cmd[0], "exit"));
}

void	handle_execution(t_shell *shell, int *pos_token)
{
	char	**cmd;

	if (shell->charge == 0)
		return ;
	cmd = tokens_to_cmd(shell->tokens, pos_token);
	if (!cmd || !cmd[0])
	{
		free_matrix(cmd);
		return ;
	}
	
	if(!is_builtin(cmd))
		execute_command(shell, cmd);
	else
		printf("is builtin!\n");
	free_matrix(cmd);
	ft_close(shell->pipe_in);
	ft_close(shell->pipe_out);
	shell->pipe_in = -1;
	shell->pipe_out = -1;
	shell->charge = 0;
}
