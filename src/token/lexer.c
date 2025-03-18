#include "minishell.h"

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
				return (0);
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
			count += count_next_token(line, &index_line);
			if (is_separator(line[index_line]))
			{
				index_line++; 
				count++;
			}
		}
	}
	return (count);
}

static t_token *gettokens(t_shell *shell, char *input_line)
{
	t_token		*tokens;
	int	numb_tokens;

	numb_tokens = count_tokens(input_line);
	printf("Numero de tokens : %d\n", numb_tokens);
	if (numb_tokens == 0)
		return (NULL);
	tokens = (t_token *)malloc(sizeof(t_token) * (numb_tokens + 1));
	if (tokens == NULL)
	{
		shell->status.last_return = 1;
		return (NULL);
	}
	process_tokens(shell, input_line, tokens, numb_tokens);
	//organize_tokens(&tokens);
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