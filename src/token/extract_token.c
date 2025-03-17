#include "minishell.h"

static bool is_matching_quote_closed(char *str, int i, int quote)
{
	while (str[++i])
	{
		if (str[i] == quote)
			return (true);
	}
	return (false);
}

void toggle_quote_state(int *in_quotes,char *input, int *index)
{
	if (input[*index] == '"' || input[*index] == '\'')
	{
		if (*in_quotes == 0)
		{
			if (is_matching_quote_closed(input, *index, input[*index]))
				*in_quotes = input[*index];
		}
		else if (*in_quotes == input[*index])
			*in_quotes = 0;
	}
}

char	*extract_token(char *input, int *index_inp)
{
	char	*token;
	int		index_tok;
	int		in_quotes;

	token = (char *)malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!token)
		return (NULL);
	index_tok = 0;
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
				token[0] = input[*index_inp];
				token[1] = '\0';
				return (token);
			}
		}
		token[index_tok++] = input[(*index_inp)++];
	}
	token[index_tok] = '\0';
	return (token);
}


/*

char *extract_token(char *input, int *index_inp, int *has_quotes)
{
	char 	*token;
	int		index_tok;
	int		is_single_quoted;
	int 	is_double_quoted;

	token = (char *)malloc(sizeof(char) * ft_strlen(input) + 1);
	if (!token)
		return (NULL);
	index_tok = 0;
	is_single_quoted = 0;
	is_double_quoted = 0;
	*has_quotes = 0;
	while (input[*index_inp])
	{
		toggle_quote_state(&is_single_quoted, &is_double_quoted, input, index_inp);
		if (is_double_quoted == 1 || is_single_quoted == 1)
			*has_quotes = 1;
		if (is_double_quoted == 0 && is_single_quoted == 0 && ((input[*index_inp] == '|' || input[*index_inp] == '>' || input[*index_inp] == '<') || input[*index_inp] == ' '))
			break ;
		token[index_tok++] = input[(*index_inp)++];
	}
	token[index_tok] = '\0';
	return (token);
}

*/