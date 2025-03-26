#include "minishell.h"

static char *get_trimmed_line(t_shell *shell, char *delimiter)
{
	char	*line;
	char	*tmp;

	line = readline("> ");
	if (line == NULL)
	{
		ft_putstr_fd("minishell: warning : here_document"
		"delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (NULL);
	}
	tmp = ft_strtrim(line, " ");
	tmp = process_expansion(shell, tmp, 1);
	ft_free(line);
	return (tmp);
}

static char	*heredoc(t_shell *shell, char *delimiter)
{
	char	*tmp;
	char	*str;
	char	*new_str;

	str = ft_strdup("");
	while (true)
	{
		tmp = get_trimmed_line(shell, delimiter);
		if (tmp == NULL)
			break ;
		if (ft_strcmp(tmp, delimiter) == 0)
		{
			ft_free(tmp);
			break ;
		}
		new_str = ft_strjoin(str, tmp);
		free(str);
		free(tmp);
		str = ft_strjoin(new_str, "\n");
		free(new_str);
	}
	return (str);
}

static int print_syntax_error(void)
{
	ft_putendl_fd("minishell: syntax error near unexpected token", 2);
	return (258);
}

static int process_delimiter(t_shell *shell, t_token *tokens, int i, char **str_heredoc)
{
	if (!tokens[i + 1].str || is_type_token(tokens[i + 1], "REDIR_OUT")
			|| is_type_token(tokens[i + 1], "REDIR_IN") || is_type_token(tokens[i + 1], "APPEND")
			|| is_type_token(tokens[i + 1], "PIPE") || is_type_token(tokens[i + 1], "HEREDOC"))
			return (print_syntax_error());
	*str_heredoc = heredoc(shell, tokens[i + 1].str);
	free(tokens[i + 1].str);
	tokens[i + 1].str = NULL;
	free(tokens[i].str);
	tokens[i].str = NULL;
	return (1);
}

static int	verifying_heredoc(t_shell *shell, t_token *tokens, char **str_heredoc)
{
	int	i;

	i = -1;
	*str_heredoc = NULL;
	while (tokens[++i].str)
		if (is_type_token(tokens[i], "HEREDOC"))
			return (process_delimiter(shell, tokens, i, str_heredoc));
	return (0);
}

void	token_analysis(t_shell *shell, char *input_line)
{
	t_token	*tokens;
	char	*str_heredoc;
	int		verify_heredoc;

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
	str_heredoc = NULL;
	verify_heredoc = verifying_heredoc(shell, tokens, &str_heredoc);
	if (verify_heredoc == 258)
	{
		shell->status.last_return = 258;
		return ;
	}
	/*shell->charge = 1;
	int test =handle_redirection(shell, 0, 0);
	if(test == 1)
		printf("Success\n");*/
	ft_free_tokens(tokens);
	ft_free(input_line);
}