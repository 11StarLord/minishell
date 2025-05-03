#include "minishell.h"

static void	redirection_input(t_shell *shell, char *file)
{
	if(shell->fd_in > 0)
		close(shell->fd_in);
	shell->fd_in = open(file, O_RDONLY, S_IRWXU);
	if (shell->fd_in == -1)
	{
		perror("minishell");
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
		return ;
	}
	if (dup2(shell->fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell");
		if(shell->fd_in > 0)
			close(shell->fd_in);
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
	}
}

static void redirection(t_shell *shell, char *file, char *type)
{
    if (!file || !type)
		return ;
	if (shell->fd_out > 0)
        close(shell->fd_out);
    if (ft_strcmp(type, "trunc") == 0)
		shell->fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    else
	    shell->fd_out = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (shell->fd_out == -1)
    {
		perror("minishell");
        shell->status.last_return = 1;
        shell->status.no_exec = 1;
        return ;
    }
    if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
    {
		perror("minishell");
        close(shell->fd_out);
        return ;
	}
}

void	handle_redirection(t_shell *shell, int token_index, int *pipe)
{
	int	test_pipe;

	if (!pipe)
		pipe = &test_pipe;
	while (shell->tokens[token_index].str)
	{
		if (is_type_token(shell->tokens[token_index], "REDIR_OUT"))
		{
			redirection(shell, shell->tokens[token_index + 1].str, "trunc");
			token_index += 2;
		}
		else if (is_type_token(shell->tokens[token_index], "APPEND"))
		{
			redirection(shell, shell->tokens[token_index + 1].str, "append");
			token_index += 2;
		}
		else if (is_type_token(shell->tokens[token_index], "REDIR_IN"))
		{
			redirection_input(shell, shell->tokens[token_index + 1].str);
			token_index += 2;
		}
		else if (is_type_token(shell->tokens[token_index], "PIPE"))
		{
			*pipe = create_pipe_process(shell);
			token_index++;
		}
		else
			token_index++;
	}
}
