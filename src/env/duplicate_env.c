#include "minishell.h"

static t_env *add_env(char *env_var)
{
	t_env	*new;
	char	**split_env;

	new = (t_env *)malloc(sizeof(t_env));
	if (new == NULL)
		return (NULL);
	if (ft_strchr(env_var, '='))
		new->equal = true;
	else
		new->equal = false;
	split_env = ft_split(env_var, '=');
	new->key = ft_strdup(split_env[0]);
	if (new->equal && split_env[1])
		new->value = ft_strdup(split_env[1]);
	else
		new->value = NULL;
	new->next = NULL;
	free_matrix(split_env);
	return(new);
}

void    duplicate_env(t_shell *shell, char **env)
{
	t_env	*new;
	int	i;

	i = -1;
	new = NULL;
	while (env[++i])
	{
		if (new == NULL)
		{
			new = add_env(env[i]);
			shell->env = new;
			//shell->env_copy = new;
		}
		else
		{
			new->next = add_env(env[i]);
			new = new->next;
		}
	}
}