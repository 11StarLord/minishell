#include "minishell.h"

static void	redirection_input(t_shell *shell, char *file)
{
	if(shell->fd_in > 0)
		close(shell->fd_in);
	shell->fd_in = open(file, O_RDONLY, S_IRWXU);
	if (shell->fd_in == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
		return ;
	}
	if (dup2(shell->fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		if(shell->fd_in > 0)
			close(shell->fd_in);
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
	}
}

static void redirection_out(t_shell *shell, char *file, char *type)
{
    if (!file || !type)
		return ;
    ft_close(shell->fd_out);
    if (ft_strcmp(type, "trunc") == 0)
		shell->fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    else
	    shell->fd_out = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (shell->fd_out == -1)
    {
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        shell->status.last_return = 1;
        shell->status.no_exec = 1;
        return ;
    }
	if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
    {
		perror("minishell: dup2");
        close(shell->fd_out);
        return ;
	}
}

void	handle_redirection(t_shell *shell, int token_index, int pipe)
{
	t_token	prev; 

	prev.str = NULL;
	if (token_index > 0)
		prev = shell->tokens[token_index - 1];
	if (is_type_token(shell->tokens[token_index], "REDIR_OUT"))
		redirection_out(shell, shell->tokens[token_index + 1].str, "trunc");
	else if (is_type_token(shell->tokens[token_index], "APPEND"))
		redirection_out(shell, shell->tokens[token_index + 1].str, "append");
	else if (is_type_token(shell->tokens[token_index], "REDIR_IN"))
		redirection_input(shell, shell->tokens[token_index + 1].str);
	else if (is_type_token(shell->tokens[token_index], "PIPE"))
		pipe = minipipe(shell);
	if (shell->tokens[(token_index + 1)].str && pipe != 1)
		handle_redirection(shell, (token_index + 1), 0);
	if ((!prev.str || is_type_token(prev, "PIPE")) && pipe != 1 && shell->status.no_exec == 0)
		handle_execution(shell, &token_index);
}