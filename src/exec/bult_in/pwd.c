/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:43:58 by achivela          #+#    #+#             */
/*   Updated: 2025/06/03 11:44:25 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pwd(t_shell *shell)
{
	char	*cwd;

	cwd = malloc(PATH_MAX);
	if (cwd == NULL)
	{
		perror("Error malloc");
		shell->status.last_return = 1;
		return (NULL);
	}
	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		shell->status.last_return = 0;
		return (cwd);
	}
	ft_free(cwd);
	shell->status.last_return = 1;
	return (NULL);
}

void	ft_pwd(char **cmd, t_shell *shell)
{
	char	*path;

	if (without_option(cmd, shell))
		return ;
	path = pwd(shell);
	if (path)
	{
		ft_putstr_fd(path, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		ft_free(path);
	}
}
