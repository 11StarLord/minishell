#include "minishell.h"

void	process_command(t_shell *shell)
{
	int	pos_token;

	pos_token = 0;
	handle_redirection(shell, 0);
	handle_execution(shell, &pos_token);
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