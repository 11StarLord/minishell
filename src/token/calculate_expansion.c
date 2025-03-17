#include "minishell.h"

bool	is_valid_token(char *str, int i, t_shell *shell)
{
	char quote = str[i];

	if (quote == '"' || quote == '\'')
	{
		if (is_quote_unclosed(str, i, quote, shell))
			return (true);
		if (!shell->tmp.active_quote || shell->tmp.active_quote == quote)
			return (false);
	}
	return (true);
}


int	handle_return_value(t_shell *shell, int *index, int *total_length)
{
	shell->tmp.str = ft_itoa(shell->status.last_return);
	*total_length += ft_strlen(shell->tmp.str);
	ft_free(shell->tmp.str);
	(*index)++;
	return (*total_length);
}

int	get_variable_length(t_shell *shell, char *input, int *index)
{
	int	i;
	int len_aloc;
	char    *env_name;
	char    *env_value;

	i = 0;
	len_aloc = 0;
	(*index)++;
	if (input[*index] == '?')
		return (handle_return_value(shell, index, &len_aloc));
	while (input[*index + i] && (ft_isalnum(input[*index + i]) || input[*index + i] == '_'))
		i++;
	if (i == 0)
		return (0);
	env_name = ft_strndup(input + *index, i);
	*index += i;
	env_value = ft_getenv(shell->env, env_name);
	ft_free(env_name);
	if (env_value)
	{
		len_aloc += ft_strlen(env_value);
		ft_free(env_value);
	}
	return (len_aloc);
}

int calculate_expanded_length(t_shell *shell, char *input, int is_heredoc)
{
	int index = 0;
	int expanded_size = 0;

	shell->tmp.active_quote = 0;
	while (input[index])
	{
		if (input[index] == '$' && input[index + 1] && 
			(shell->tmp.active_quote != '\'' || is_heredoc))
		{ 
			expanded_size += get_variable_length(shell, input, &index);
		}
		else
		{
			if (is_valid_token(input, index, shell))
				expanded_size++;
			handle_quotes(shell, input, &index);
			index++;
		}
	}
	return (expanded_size);
}