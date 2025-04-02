#include "minishell.h"

char	**cmd_tab(t_token *tokens, int *pos_token)
{
	char	**tab;
	int		i;
	int		j;

	if (!tokens)
		return (NULL);
	i = *pos_token;
	j = 0;
	while (tokens[i].str && compare_type(tokens[i], "ARGUMENT"))
	{
		i++;
		j++;
	}
	tab = malloc(sizeof(char *) * (j + 1));
	if (!tab)
		return (NULL);
	ft_memset(tab, 0, sizeof(char *) * (j + 1));
	i = 0;
	while (tokens[*pos_token].str && compare_type(tokens[*pos_token], "ARGUMENT"))
	{
		if (ft_strcmp(tokens[*pos_token].str, ""))
			tab[i++] = ft_strdup(tokens[*pos_token].str);
		else
			tab[i++] = ft_strdup(" ");
		*pos_token += 1;
	}
	{
		tab[i++] = ft_strdup(tokens[*pos_token].str);
		*pos_token += 1;
	}
	tab[i] = NULL;
	return (tab);
}

void	handle_execution(t_shell *shell, int *pos_token)
{
	char	**cmd;

	if (shell->charge == 0)
		return ;
	cmd = cmd_tab(shell->tokens, pos_token);
	if (!cmd[0])
		return ;
	if (ft_strlen(cmd[0]) == 0)
	{
		ft_putstr_fd("minishell: command '' not found\n", STDERR_FILENO);
		shell->status.no_exec = 1;
		shell->status.last_return = 127;
		free_matrix(cmd);
		return ;
	}

	printf("Executing command: %s\n", cmd[0]);
	free_matrix(cmd);
	close(shell->pipe_in);
	close(shell->pipe_out);
}
