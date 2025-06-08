/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achivela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:31:40 by achivela          #+#    #+#             */
/*   Updated: 2025/05/30 16:31:54 by achivela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc,char **argv, char **env)
{
    t_shell shell;
    (void)argc;
    (void)argv;
    init_shell(&shell, env);
    return(shell.status.last_return);
}

