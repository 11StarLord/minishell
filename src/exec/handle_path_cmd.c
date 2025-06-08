#include "minishell.h"

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

char *get_command_path(char *cmd, t_shell *shell)
{
	char	**path_dirs;
	char	*result;

    result = NULL;

    if (cmd[0] == '/' || cmd[0] == '.')
    {
         if (access(cmd, X_OK) == 0)
             result = (ft_strdup(cmd));
    }

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