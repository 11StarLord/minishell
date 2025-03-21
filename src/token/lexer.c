#include "minishell.h"

t_token	*organize_tokens(t_token *tokens)
{
	int		index_tok;
	t_token	tmp_token[3];

	//printf("Entreiiiiiiiiiii\n");
	index_tok = -1;
	while (tokens[++index_tok].str)
	{
		if (is_type_token(tokens[index_tok], "REDIR_OUT")
			|| is_type_token(tokens[index_tok], "REDIR_IN")
			|| is_type_token(tokens[index_tok], "APPEND")
			|| is_type_token(tokens[index_tok], "HEREDOC"))
		{
			if (tokens[index_tok + 2].str
				&& is_type_token(tokens[index_tok + 2], "ARGUMENT"))
			{
				tmp_token[0] = tokens[index_tok];
				tmp_token[1] = tokens[index_tok + 1];
				tmp_token[2] = tokens[index_tok + 2];
				tokens[index_tok] = tmp_token[2];
				tokens[index_tok + 1] = tmp_token[0];
				tokens[index_tok + 2] = tmp_token[1];
				index_tok = 0;
			}
		}
	}
	index_tok = 0;
	while (tokens[index_tok++].str)
	{
		printf("%d\t%s\t%s\n", index_tok, tokens[index_tok].str,tokens[index_tok].type);
	}
	return (tokens);
}

static int count_next_token(char *input, int *index_inp)
{
	int		found_token;
	int		in_quotes;

	found_token = 0;
	in_quotes = 0;
	while (input[*index_inp])
	{
		toggle_quote_state(&in_quotes, input, index_inp);
		if (in_quotes == 0)
		{
			if (input[*index_inp] == ' ')
				break;
			if (is_separator(input[*index_inp]))
			{
				if (found_token)
					return (1);
				else
					return (0);
			}
		}
		found_token = 1;
		(*index_inp)++;
	}
	return (found_token);
}

static int count_tokens(char *line)
{
	int index_line;
	int count;

	index_line = 0;
	count = 0;
	while (line[index_line])
	{
		skip_whitespace(line, &index_line);
		if (line[index_line])
		{
			skip_whitespace(line, &index_line);
			if (is_separator(line[index_line]))
			{
				index_line++; 
				count++;
			}
			count += count_next_token(line, &index_line);
		}
	}
	return (count);
}

static t_token *gettokens(t_shell *shell, char *input_line)
{
	t_token		*tokens;
	int	numb_tokens;

	numb_tokens = count_tokens(input_line);
	if (numb_tokens == 0)
		return (NULL);
	tokens = (t_token *)malloc(sizeof(t_token) * (numb_tokens + 1));
	if (tokens == NULL)
	{
		shell->status.last_return = 1;
		return (NULL);
	}
	process_tokens(shell, input_line, tokens, numb_tokens);
	organize_tokens(tokens);
	return (tokens);
}

void	lexical_analysis(t_shell *shell, char *input_line)
{
	t_token	*tokens;

	input_line = ft_strtrim(input_line, " ");
	if (!input_line || !input_line[0])
	{
		ft_free(input_line);
		return ;
	}
	tokens = gettokens(shell, input_line);
	if (!tokens)
	{
		ft_free(input_line);
		shell->status.last_return = 1;
		return ;
	}
	/*process_and_validate_line(shell, &tokens);
	if (shell->status.last_return == 258)
	{
		ft_free(input_line);
		return ;
	}
	shell->charge = 1;
	redir_and_exec(shell, 0, 0);*/
	ft_free_tokens(tokens);
	ft_free(input_line);
}