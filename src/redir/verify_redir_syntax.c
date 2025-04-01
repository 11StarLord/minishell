#include "minishell.h"

static bool is_valid_previous_token(t_token *tokens, int i)
{
	if (tokens[i - 1].str)
	{
		if (is_type_token(tokens[i - 1], "ARGUMENT"))
			return (true);
	}
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(tokens[i].str, 2);
    ft_putstr_fd("'\n", 2);
	return (false);
}

static bool is_valid_next_token(t_token *tokens, int i)
{
	if (tokens[i + 1].str)
	{
		if (is_type_token(tokens[i + 1], "ARGUMENT"))
			return (true);
	}
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(tokens[i].str, 2);
    ft_putstr_fd("'\n", 2);
	return (false);
}

bool is_valid_redirect_syntax(t_token *tokens, t_shell *shell)
{
	int	i;

	i = 0;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "REDIR_OUT")
		|| is_type_token(tokens[i], "REDIR_IN") || is_type_token(tokens[i], "APPEND")
		|| is_type_token(tokens[i], "PIPE") || is_type_token(tokens[i], "HEREDOC"))
		{
			if (!is_valid_next_token(tokens, i)|| !is_valid_previous_token(tokens, i))
			{
				shell->status.last_return = 258;
				return (false);
			}
		}
		i++;
	}
	return (true);
}