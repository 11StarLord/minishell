#include "minishell.h"

int ft_readline(t_shell *shell, char **line)
{
    *line = readline("minishell$ ");
    if (!*line)
    {
        shell->status.exit_status = 1;
        ft_putendl_fd("exit", STDERR_FILENO);
        return (0);
    }
    if(**line)
        add_history(*line);
    return (1);
}