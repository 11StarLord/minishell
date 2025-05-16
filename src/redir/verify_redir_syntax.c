#include "minishell.h"

static bool	is_valid_pipe(t_token *tokens, int i)
{
	if (tokens[i + 1].str)
	{
		if (!is_type_token(tokens[i + 1], "ARGUMENT"))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			return (false);
		}
		if (is_type_token(tokens[i + 1], "ARGUMENT") && i == 0)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			return (false);
		}
		return (true);
	}
	if (i > 0)
	{
		if (is_type_token(tokens[i - 1], "ARGUMENT"))
		{
			printf("Tratar\n");
			return (true);
		}
	}
	ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
	return (false);
}

static bool is_valid_next_token(t_token *tokens, int i)
{
	if (tokens[i + 1].str)
	{
		if (is_type_token(tokens[i + 1], "ARGUMENT"))
			return (true);
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
			ft_putstr_fd(tokens[i + 1].str, STDERR_FILENO);
			ft_putstr_fd("'\n", STDERR_FILENO);
			return (false);
		}
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (false);
	}
}

bool is_valid_redirect_syntax(t_token *tokens, t_shell *shell)
{
	int	i;

	i = 0;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "REDIR_IN")
			|| is_type_token(tokens[i], "REDIR_OUT")
			|| (is_type_token(tokens[i], "APPEND")))
		{
			if (!is_valid_next_token(tokens, i))
			{
				shell->status.last_return = 2;
				return (false);
			}
		}
		if (is_type_token(tokens[i], "PIPE"))
		{
			if (!is_valid_pipe(tokens, i))
			{
				shell->status.last_return = 2;
				return (false);
			}
		}
		i++;
	}
	return (true);
}