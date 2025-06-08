/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:05:16 by achivela          #+#    #+#             */
/*   Updated: 2025/06/07 18:05:19 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expanded(char *args, t_env **env)
{
	t_env	*env_tmp;
	t_env	*prev;

	env_tmp = *env;
	prev = NULL;
	while (env_tmp)
	{
		if (env_tmp->value && ft_strcmp(env_tmp->value, args) == 0)
		{
			if (!prev)
				*env = env_tmp->next;
			else
				prev->next = env_tmp->next;
			ft_free(env_tmp->key);
			ft_free(env_tmp->value);
			ft_free(env_tmp);
			break ;
		}
		prev = env_tmp;
		env_tmp = env_tmp->next;
	}
}

void	non_expanded(char *args, t_env **env)
{
	t_env	*env_tmp;
	t_env	*prev;

	env_tmp = *env;
	prev = NULL;
	while (env_tmp)
	{
		if (ft_strcmp(env_tmp->key, args) == 0)
		{
			if (!prev)
				*env = env_tmp->next;
			else
				prev->next = env_tmp->next;
			ft_free(env_tmp->key);
			ft_free(env_tmp->value);
			ft_free(env_tmp);
			break ;
		}
		prev = env_tmp;
		env_tmp = env_tmp->next;
	}
}

void	ft_unset(char **cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmd[i] && cmd[i + 1] == NULL)
	{
		if (ft_isalpha(cmd[i][0]) || cmd[i][0] == '_')
			non_expanded(cmd[i], &shell->env);
		else
			expanded(cmd[i], &shell->env);
	}
	return ;
}
