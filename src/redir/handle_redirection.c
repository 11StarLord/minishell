#include "minishell.h"

static void	redir(t_shell *shell, char *file, char *type)
{
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
	dup2(shell->out ,shell->fd_out);
}


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
	dup2(shell->in ,shell->fd_in);
}

/*static void redirection(t_shell *shell, char *file, char *type)
{
	printf("Entrei\n");
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
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(file, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        shell->status.last_return = 1;
        shell->status.no_exec = 1;
        return ;
    }
    if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
    {
        ft_putstr_fd("minishell: error: failed to redirect output\n", STDERR_FILENO);
        close(shell->fd_out);
        return ;
	}
}*/


void	handle_redirection(t_shell *shell, int token_index, int *pipe)
{
	(void)pipe;
	while (shell->tokens[token_index].str)
	{
		if (is_type_token(shell->tokens[token_index], "REDIR_OUT"))
			redir(shell, shell->tokens[token_index + 1].str, "trunc");
		else if (is_type_token(shell->tokens[token_index], "APPEND"))
			redir(shell, shell->tokens[token_index + 1].str, "append");
		else if (is_type_token(shell->tokens[token_index], "REDIR_IN"))
			redirection_input(shell, shell->tokens[token_index + 1].str);
		token_index++;
	}
    	/*
	else if (compare_type(previous_token, "PIPE"))
		*pipe = create_pipe_process(shell);

	if (shell->tokens[token_index + 1].str && *pipe != 1)
		handle_redirection(shell, token_index + 1, pipe);*/
	
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
