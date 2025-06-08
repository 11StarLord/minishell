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

static char *format_env_value(char *key, char *value, int equal, bool has_quote)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strdup(key);
	if (!tmp)
		return (NULL);
	if (value && equal == 1)
	{
		joined = ft_strjoin(tmp, "=");
		ft_free(tmp);
		tmp = append_value(joined, value, has_quote == true);
	}
	return (tmp);
}

char **env_to_matrix(t_env *env, int i, bool has_quote)
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