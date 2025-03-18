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

char	*extract_token(char *input, int *index_inp, int *in_quotes)
{
	char	*token;
	int		index_tok;

	token = (char *)malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!token)
		return (NULL);
	index_tok = 0;
	*in_quotes = 0;
	while (input[*index_inp])
	{
		toggle_quote_state(in_quotes, input, index_inp);
		if (*in_quotes == 0)
		{
			if (input[*index_inp] == ' ')
				break;
			if (is_separator(input[*index_inp]))
			{
				if (index_tok == 0)
				{
					token[0] = input[*index_inp];
					token[1] = '\0';
					return (token);
				}
				break ;
			}
		}
		token[index_tok++] = input[(*index_inp)++];
	}
	token[index_tok] = '\0';
	return (token);
}
