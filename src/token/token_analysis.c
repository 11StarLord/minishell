#include "minishell.h"

static void copy_remaining_tokens(t_shell *shell, t_token *tokens, int *i)
{
	int	j;

	j = 0;
	if (tokens[0].str)
	{
		shell->tokens[*i].str = ft_strdup("|");
		shell->tokens[(*i)++].type = "PIPE";
	}
	while (tokens[j].str)
	{
		shell->tokens[*i].str = ft_strdup(tokens[j].str);
		shell->tokens[*i].type = tokens[j].type;
		(*i)++;
		j++;
	}
	shell->tokens[*i].str = NULL;
}

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

	i = 0;
	if (str_heredoc && str_heredoc[0])
	{
		shell->tokens = (t_token *)malloc(sizeof(t_token) * (shell->tokens_size + 4));
		add_heredoc_tokens(shell, str_heredoc, &i);
		copy_remaining_tokens(shell, tokens, &i);
	}
	else
	{
		shell->tokens = (t_token *)malloc(sizeof(t_token) * (shell->tokens_size + 1));
		while (tokens[i].str)
		{
			shell->tokens[i].str = ft_strdup(tokens[i].str);
			shell->tokens[i].type = tokens[i].type;
			i++;
		}
		shell->tokens[i].str = NULL;
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
	/*shell->charge = 1;
	int test =handle_redirection(shell, 0, 0);
	if(test == 1)
		printf("Success\n");
	else
		printf("Failed\n");
	ft_free_tokens(tokens);
	ft_free(input_line);*/
}