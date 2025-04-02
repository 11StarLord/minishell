#include "minishell.h"

static void	redirection_input(t_shell *shell, const char *file)
{
	if(shell->fd_in > 0)
		close(shell->fd_in);

	shell->fd_in = open(file, O_RDONLY);
	if (shell->fd_in == -1)
	{
		perror("minishell: erro ao abrir arquivo de entrada");
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
		return;
	}

	if (dup2(shell->fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell: erro ao redirecionar entrada");
		if(shell->fd_in > 0)
			close(shell->fd_in);
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
	}
}

static int redirection(t_shell *shell, char *file, char *type)
{
    if (!file || !type)
        return (-1);

    int flags = O_CREAT | O_WRONLY;
    if (ft_strcmp(type, "trunc") == 0)
        flags |= O_TRUNC;
    else
        flags |= O_APPEND;

    if (shell->fd_out > 0)
        close(shell->fd_out);

    shell->fd_out = open(file, flags, S_IRWXU);
    if (shell->fd_out == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(file, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        shell->status.last_return = 1;
        shell->status.no_exec = 1;
        return (-1);
    }

    if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
    {
        ft_putstr_fd("minishell: error: failed to redirect output\n", STDERR_FILENO);
        close(shell->fd_out);
        return (-1);
    }

    return (0);
}


void	handle_redirection(t_shell *shell, int token_index, int *pipe)
{
	t_token previous_token;

	previous_token.str = NULL;
	if (token_index > 0)
		previous_token = shell->tokens[token_index - 1];

    if (compare_type(previous_token, "REDIR_OUT"))
		redirection(shell, shell->tokens[token_index].str, "trunc");
	else if (compare_type(previous_token, "APPEND"))
		redirection(shell, shell->tokens[token_index].str, "append");
	else if (compare_type(previous_token, "REDIR_IN"))
        redirection_input(shell, shell->tokens[token_index].str);
	else if (compare_type(previous_token, "PIPE"))
		*pipe = create_pipe_process(shell);

	if (shell->tokens[token_index + 1].str && *pipe != 1)
		handle_redirection(shell, token_index + 1, pipe);
}

void	execute_command(t_shell *shell, int token_index, int pipe)
{
	t_token previous_token;

	previous_token.str = NULL;
	if (token_index > 0)
		previous_token = shell->tokens[token_index - 1];

	if ((!previous_token.str || compare_type(previous_token, "PIPE")) && pipe != 1 && shell->status.no_exec == 0)
        handle_execution(shell, &token_index);
}

