#include "minishell.h"

char	*ft_getenv(t_env *env, char *var)
{
	t_env	*current_env;

	current_env = env;
	while (current_env)
	{
		if (ft_strcmp(current_env->key, var) == 0)
		{
			if (current_env->equal == 1 && current_env->value)
				return (ft_strdup(current_env->value));
			else
				return (NULL);
		}
		current_env = current_env->next;
	}
	return (NULL);
}