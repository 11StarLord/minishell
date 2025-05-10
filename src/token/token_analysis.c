#include "minishell.h"

bool	is_redirection_token(t_token token)
{
	return (is_type_token(token, "REDIR_OUT") || 
		is_type_token(token, "APPEND") ||
		is_type_token(token, "REDIR_IN") );
}

void	process_command(t_shell *shell)
{
	int	pos_token;

	pos_token = 0;
	while (shell->tokens[pos_token].str)
	{
		while (is_redirection_token(shell->tokens[pos_token]))
		{
			handle_redirection(shell, &pos_token);
			pos_token++;
		}
		if (is_type_token(shell->tokens[pos_token], "ARGUMENT"))
		{
			pos_token = 0;
			handle_execution(shell, &pos_token);
		}
		if (is_type_token(shell->tokens[pos_token], "ARGUMENT"))
		{
			create_pipe_process(shell);
		}
		pos_token++;
	}
}

void	token_analysis(t_shell *shell, char *input_line)
{
	t_token	*tokens;

	input_line = ft_strtrim(input_line, " ");
	if (!input_line || !input_line[0])
	{
		ft_free(input_line);
		return ;
	}
	gettokens(shell, input_line,&tokens);
	if (!tokens)
	{
		ft_free(input_line);
		shell->status.last_return = 1;
		return ;
	}
	if(is_valid_redirect_syntax(tokens, shell) == false)
	{
		ft_free(input_line);
		ft_free_tokens(tokens);
		return ;
	}
	dup_tokens(shell,tokens);
	shell->charge = 1;
	process_command(shell);
    ft_free_tokens(tokens);
    ft_free(input_line);
}