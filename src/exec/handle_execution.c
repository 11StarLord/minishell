#include "minishell.h"

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

static int	is_builtin(char **cmd)
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
		execute_builtin(shell, cmd); 
	free_matrix(cmd);
	ft_close(shell->pipe_in);
	ft_close(shell->pipe_out);
	shell->pipe_in = -1;
	shell->pipe_out = -1;
	shell->charge = 0;
}
