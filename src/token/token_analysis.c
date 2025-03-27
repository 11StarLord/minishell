#include "minishell.h"

static void add_heredoc_tokens(t_shell *shell, char *str_heredoc, int *i)
{
	shell->tokens[*i].str = ft_strdup("/bin/echo/");
	shell->tokens[(*i)++].type = "ARGUMENT";
	shell->tokens[*i].str = ft_strdup("-e");
	shell->tokens[(*i)++].type = "ARGUMENT";
	shell->tokens[*i].str = ft_strdup(str_heredoc);
	shell->tokens[(*i)++].type = "ARGUMENT";
}

static void	dup_tokens(t_shell *shell, t_token *tokens, char *str_heredoc)
{
	int	i;
	int	numb_tokens;

	i = 0;
	numb_tokens = 0;
	while (tokens[i].str)
	{
		numb_tokens++;
		i++;
	}
	shell->tokens = (t_token *)malloc(sizeof(t_token) * (numb_tokens + 1));
	if (!shell->tokens)
		return ;
	i = 0;
	if (str_heredoc && str_heredoc[0])
	{
		add_heredoc_tokens(shell, str_heredoc, &i);
	}
}















static void process_and_validate_line(t_shell *shell, t_token **tokens)
{
	char	*str_heredoc;
	int		verify_heredoc;

	str_heredoc = NULL;
	verify_heredoc = verifying_heredoc(shell, *tokens, &str_heredoc);
	dup_tokens(shell, *tokens, str_heredoc);
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