#include "minishell.h"

char	*get_env_value(t_shell *shell, char *input, int *index_inp)
{
	char	*env_name;
	char	*env_value;
	int		len;

	if (input[*index_inp] == '?')
	{
		(*index_inp)++;
		return (ft_itoa(shell->status.last_return));
	}
	len = 0;
	while (input[*index_inp + len] && (ft_isalnum(input[*index_inp + len]) || input[*index_inp + len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	env_name = ft_strndup(input + *index_inp, len);
	*index_inp += len;
	env_value = ft_getenv(shell->env, env_name);
	ft_free(env_name);
	if (env_value)
		return (env_value);
	else
		return (NULL);
}

void    handle_variable_expansion(t_shell *shell, char *expanded, int *index_inp, int *index_exp)
{
    char    *env_value;
    int i;

    (*index_inp)++;
	if (ft_strcmp(shell->tmp.str, "\"$\"") == 0)
		env_value = ft_strdup("$");
	else
		env_value = get_env_value(shell, shell->tmp.str, index_inp);
	if (env_value)
	{
		i = 0;
		while (env_value[i])
			expanded[(*index_exp)++] = env_value[i++];
	}
	ft_free(env_value);
}

void	perform_variable_expansion(t_shell *shell, char *input, char *expanded, int in_heredoc)
{
	int	index_inp;
	int	index_exp;

	index_inp = 0;
	index_exp = 0;
	shell->tmp.active_quote = 0;
	if (!expanded)
		return ;
	while (input[index_inp])
	{
		if (input[index_inp] == '$' && input[index_inp + 1]
			&& (shell->tmp.active_quote != '\'' || in_heredoc))
		{
			shell->tmp.str = input;
			handle_variable_expansion(shell, expanded, &index_inp, &index_exp);
		}
		else
		{
			if (is_valid_token(input, index_inp, shell))
				expanded[index_exp++] = input[index_inp];
			handle_quotes(shell, input, &index_inp);
			index_inp++;
		}
	}
	expanded[index_exp] = '\0';
}

char	*process_expansion(t_shell *shell, char *input, int is_heredoc)
{
    char	*expanded;
    int		expanded_size;

    if (input[0] == '$' && input[1] == '$')
        return (input);
    expanded_size = calculate_expanded_length(shell, input, is_heredoc);
    expanded = (char *)malloc(expanded_size + 1);
    if (!expanded)
    {
        ft_free(input);
        return NULL;
    }
    perform_variable_expansion(shell, input, expanded, is_heredoc);
    ft_free(input);
    return expanded;
}
