/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:10:31 by achivela          #+#    #+#             */
/*   Updated: 2025/06/05 11:10:33 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	verify_is_number(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	verify_many_arg(char **cmd, t_shell *shell, int num_args)
{
	if (num_args >= 3)
	{
		if (verify_is_number(cmd[1]))
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDOUT_FILENO);
			shell->status.last_return = 1;
			return (1);
		}
		else
		{
			printf("minishell: exit: %s: numeric argument required\n", cmd[1]);
			shell->status.last_return = 1;
		}
		shell->status.last_return = 2;
		shell->status.exit_status = 1;
		return (1);
	}
	return (0);
}

void	ft_exit(char **cmd, t_shell *shell)
{
	int	num_args;

	num_args = total_arg(cmd);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (verify_many_arg(cmd, shell, num_args))
		return ;
	if (num_args == 2)
	{
		if (verify_is_number(cmd[1]))
		{
			shell->status.last_return = (ft_atoi(cmd[1]) % 256);
			shell->status.exit_status = 1;
			return ;
		}
		shell->status.last_return = 2;
		shell->status.exit_status = 1;
		return ;
	}
	shell->status.exit_status = 1;
	return ;
}
