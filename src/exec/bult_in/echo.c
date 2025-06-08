/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:22:33 by achivela          #+#    #+#             */
/*   Updated: 2025/06/03 10:22:35 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	verify_break(char **option, int *i, bool *put_break)
{
	int	j;
	int	is_valid_option;

	while (option[*i] && option[*i][0] == '-')
	{
		j = 1;
		is_valid_option = 1;
		while (option[*i][j])
		{
			if (option[*i][j] != 'n')
			{
				is_valid_option = 0;
				break ;
			}
			j++;
		}
		if (!is_valid_option || j == 1)
			break ;
		(*i)++;
	}
	if (*i != 1)
		*put_break = false;
}

void	ft_echo(char **cmd, t_shell *shell)
{
	bool	put_break;
	int		i;

	i = 1;
	put_break = true;
	verify_break(cmd, &i, &put_break);
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], STDOUT_FILENO);
		if (cmd[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (put_break == true)
		ft_putchar_fd('\n', STDOUT_FILENO);
	shell->status.last_return = 0;
	shell->is_parent_process = true;
}
