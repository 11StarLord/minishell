#include "minishell.h"

static void process_and_validate_line(t_shell *shell, t_token **tokens)
{
	char	*str_heredoc;
	int		verify_heredoc;

	str_heredoc = NULL;
	verify_heredoc = verifying_heredoc(shell, *tokens, &str_heredoc);
	if (verify_heredoc == 258)
	{
		shell->status.last_return = 258;
		ft_free(tokens);
		return ;
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
	process_and_validate_line(shell, &tokens);
	if (shell->status.last_return == 258)
	{
		ft_free(input_line);
		return ;
	}
	shell->charge = 1;
	int test =handle_redirection(shell, 0, 0);
	if(test == 1)
		printf("Success\n");
	else
		printf("Failed\n");
	/*ft_free_tokens(tokens);
	ft_free(input_line);*/
}