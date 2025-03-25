#include "minishell.h"

void	redirection(t_shell *shell, char *file, char *type)
{
	ft_close(shell->fdout);
	if (ft_strcmp(type, "trunc") == 0)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	else
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	if (shell->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(file, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		shell->status->last_return = 1;
		shell->status->no_exec = 1;
		return ;
	}
	dup2(shell->fdout, STDOUT);
}

int	handle_redirection(t_shell *shell, int token_index, int is_pipe)
{
	t_token	previous_token;

	previous_token.str = NULL;
	if (token_index > 0)
		previous_token = shell->tokens[token_index - 1];

	if (compare_type(previous_token, 'REDIR_IN'))
		redirection(shell, shell->tokens[token_index].str, "trunc");
	else if (compare_type(previous_token, 'APPEND'))
		redirection(shell, shell->tokens[token_index].str, "append");
	else if (compare_type(previous_token, 'REDIR_OUT'))
		input(shell, shell->tokens[token_index].str);
	else if (compare_type(previous_token, 'PIPE'))
		is_pipe = minipipe(shell);

	if (shell->tokens[token_index + 1].str && is_pipe != 1)
		handle_redirection(shell, token_index + 1, 0);

	if ((!previous_token.str || compare_type(previous_token, 'PIPE'))
			 && is_pipe != 1 && shell->status.no_exec == 0)
		return (1);
	
	return (0);
}

