#include "minishell.h"

bool	is_quote_unclosed(char *str, int i, char quote, t_shell *shell)
{
	if (shell->tmp.active_quote == quote)
		return (false);
	while (str[++i])
	{
		if (str[i] == quote && shell->tmp.active_quote != quote)
			return (false);
	}
	return (true);
}

bool	ft_is_closed(char *str, int i, char quote)
{
	while (str[++i])
	{
		if (str[i] == quote)
			return (true);
	}
	return (false);
}

void	handle_quotes(t_shell *shell, char *input, int *index_inp)
{
	char quote = input[*index_inp];

	if ((quote == '"' || quote == '\'') && shell->tmp.active_quote != quote)
	{
		if (ft_is_closed(input, *index_inp, quote))
			shell->tmp.active_quote = quote;
	}
	else if (quote == shell->tmp.active_quote)
		shell->tmp.active_quote = 0;
}