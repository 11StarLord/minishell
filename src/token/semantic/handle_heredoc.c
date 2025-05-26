#include "minishell.h"

static void copy_remaining_tokens(t_shell *shell, t_token *tokens, int *i)
{
	int	j;

	j = 0;
	while (tokens[j].str)
	{
		if (is_type_token(tokens[j], "HEREDOC"))
		{
			j += 2;
			continue;
		}
		
		if (j == 0 || (j > 0 && is_type_token(tokens[j-1], "HEREDOC")))
		{
			shell->tokens[*i].str = ft_strdup("|");
			shell->tokens[*i].type = ft_strdup("PIPE");
			(*i)++;
		}
		
		shell->tokens[*i].str = ft_strdup(tokens[j].str);
		shell->tokens[*i].type = ft_strdup(tokens[j].type);
		(*i)++;
		j++;
	}
	shell->tokens[*i].str = NULL;
	shell->tokens[*i].type = NULL;
}

static void add_heredoc_tokens(t_shell *shell, char *str_heredoc, int *i)
{
	shell->tokens[*i].str = ft_strdup("/bin/echo");
	shell->tokens[*i].type = ft_strdup("ARGUMENT");
	(*i)++;
	shell->tokens[*i].str = ft_strdup("-e");
	shell->tokens[*i].type = ft_strdup("ARGUMENT");
	(*i)++;
	shell->tokens[*i].str = ft_strdup(str_heredoc);
	shell->tokens[*i].type = ft_strdup("ARGUMENT");
	(*i)++;
}

static char *get_trimmed_line(t_shell *shell, char *delimiter)
{
	char	*line;
	char	*tmp;

	line = readline("> ");
	if (line == NULL)
	{
		ft_putstr_fd("minishell: warning: here-document "
		"delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (NULL);
	}
	tmp = ft_strtrim(line, " ");
	free(line);
	tmp = process_expansion(shell, tmp, 1);
	return (tmp);
}

static char	*heredoc(t_shell *shell, char *delimiter)
{
	char	*tmp;
	char	*str;
	char	*new_str;
	int		len;

	str = ft_strdup("");
	if (!str)
		return (NULL);
	
	while (true)
	{
		tmp = get_trimmed_line(shell, delimiter);
		if (tmp == NULL)
			break ;
		if (ft_strcmp(tmp, delimiter) == 0)
		{
			free(tmp);
			break ;
		}
		new_str = ft_strjoin(str, tmp);
		if (!new_str)
		{
			free(str);
			free(tmp);
			return (NULL);
		}
		free(str);
		free(tmp);
		str = ft_strjoin(new_str, "\n");
		if (!str)
		{
			free(new_str);
			return (NULL);
		}
		free(new_str);
	}
	
	// Remover o último \n se existir
	if (str && str[0])
	{
		len = ft_strlen(str);
		if (len > 0 && str[len - 1] == '\n')
			str[len - 1] = '\0';
	}
	
	return (str);
}
static int count_non_heredoc_tokens(t_token *tokens)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "HEREDOC"))
		{
			i += 2;
			continue;
		}
		count++;
		i++;
	}
	return (count);
}

static bool has_heredoc_token(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "HEREDOC"))
			return (true);
		i++;
	}
	return (false);
}

bool	has_heredoc(t_shell *shell, t_token *tokens)
{
	int		i;
	int		new_tokens_count;
	char	*str_heredoc;
	t_token	*old_tokens;

	if (!has_heredoc_token(tokens))
		return (false);

	i = 0;
	str_heredoc = NULL;
	
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "HEREDOC") && tokens[i + 1].str)
		{
			str_heredoc = heredoc(shell, tokens[i + 1].str);
			break;
		}
		i++;
	}
	
	if (!str_heredoc)
		return (false);
	
	new_tokens_count = count_non_heredoc_tokens(tokens);
	if (new_tokens_count > 0)
		new_tokens_count += 4;
	else
		new_tokens_count = 4;

	old_tokens = shell->tokens;
	
	shell->tokens = (t_token *)malloc(sizeof(t_token) * new_tokens_count);
	if (!shell->tokens)
	{
		shell->tokens = old_tokens;
		free(str_heredoc);
		return (false);
	}
	
	for (int j = 0; j < new_tokens_count; j++)
	{
		shell->tokens[j].str = NULL;
		shell->tokens[j].type = NULL;
	}
	
	i = 0;
	add_heredoc_tokens(shell, str_heredoc, &i);
	
	
	if (count_non_heredoc_tokens(tokens) > 0)
		copy_remaining_tokens(shell, tokens, &i);
	

	if (old_tokens && old_tokens != tokens)
	{
		for (int j = 0; old_tokens[j].str; j++)
		{
			free(old_tokens[j].str);
			if (old_tokens[j].type)
				free(old_tokens[j].type);
		}
		free(old_tokens);
	}
	
	free(str_heredoc);
	return (true);
}