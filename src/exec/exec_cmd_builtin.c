#include "minishell.h"


void	execute_builtin(t_shell *shell, char **cmd)
{
	if (ft_strcmp(cmd[0], "echo") == 0)
		ft_echo(cmd, shell);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		ft_pwd(cmd, shell);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		ft_exit(cmd, shell);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		ft_unset(cmd, shell);
}
