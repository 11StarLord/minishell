#include "minishell.h"

static void copy_remaining_tokens(t_shell *shell, t_token *tokens, int *i)
{
	int	j;

	j = 0;
	while (tokens[j].str)
	{
		if (j == 0)
		{
			shell->tokens[*i].str = ft_strdup("|");
			shell->tokens[(*i)++].type = "PIPE";
		}
		if (is_type_token(tokens[j], "HEREDOC"))
		{
			j += 2;
			continue;
		}
		shell->tokens[*i].str = ft_strdup(tokens[j].str);
		shell->tokens[*i].type = tokens[j].type;
		(*i)++;
		j++;
	}
	shell->tokens[*i].str = NULL;
}

static void add_heredoc_tokens(t_shell *shell, char *str_heredoc, int *i)
{
	shell->tokens[*i].str = ft_strdup("/bin/echo");
	shell->tokens[(*i)++].type = "ARGUMENT";
	shell->tokens[*i].str = ft_strdup("-e");
	shell->tokens[(*i)++].type = "ARGUMENT";
	shell->tokens[*i].str = ft_strdup(str_heredoc);
	shell->tokens[(*i)++].type = "ARGUMENT";
}

static char *get_trimmed_line(t_shell *shell, char *delimiter)
{
	char	*line;
	char	*tmp;

	line = readline("> ");
	if (line == NULL)
	{
		ft_putstr_fd("minishell: warning : here_document"
		"delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (NULL);
	}
	tmp = ft_strtrim(line, " ");
	tmp = process_expansion(shell, tmp, 1);
	ft_free(line);
	return (tmp);
}

static char	*heredoc(t_shell *shell, char *delimiter)
{
	char	*tmp;
	char	*str;
	char	*new_str;

	str = ft_strdup("");
	while (true)
	{
		tmp = get_trimmed_line(shell, delimiter);
		if (tmp == NULL)
			break ;
		if (ft_strcmp(tmp, delimiter) == 0)
		{
			ft_free(tmp);
			break ;
		}
		new_str = ft_strjoin(str, tmp);
		free(str);
		free(tmp);
		str = ft_strjoin(new_str, "\n");
		free(new_str);
	}
	return (str);
}

bool	has_heredoc(t_shell *shell, t_token *tokens)
{
	int	i;
	char	*str_heredoc;

	i = 0;
	str_heredoc = NULL;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "HEREDOC") && tokens[i + 1].str)
			str_heredoc = heredoc(shell, tokens[i + 1].str);
		i++;
	}
	if (is_type_token(tokens[0], "HEREDOC") == true)
	{
		ft_free(str_heredoc);
		return (false);
	}
	if (str_heredoc && str_heredoc[0])
	{
		i = 0;
		shell->tokens = (t_token *)malloc(sizeof(t_token) * (shell->tokens_size + 3));
		add_heredoc_tokens(shell, str_heredoc, &i);
		copy_remaining_tokens(shell, tokens, &i);
		return (true);
	}
	return (false);
}