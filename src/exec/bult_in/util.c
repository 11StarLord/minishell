/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:09:21 by achivela          #+#    #+#             */
/*   Updated: 2025/06/05 18:09:23 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	without_option(char **cmd, t_shell *shell)
{
	if (cmd[1] && cmd[1][0] == '-')
	{
		printf("minishell: %s: no option required\n", cmd[0]);
		shell->status.last_return = 2;
		return (1);
	}
	return (0);
}

int	total_arg(char **cmd)
{
	int	num;

	num = 0;
	while (cmd[num])
		num ++;
	return (num);
}
