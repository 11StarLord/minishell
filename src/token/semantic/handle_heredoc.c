#include "minishell.h"

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

/*static int process_delimiter(t_shell *shell, t_token *tokens, int i, char **str_heredoc)
{
	*str_heredoc = heredoc(shell, tokens[i + 1].str);
	free(tokens[i + 1].str);
	tokens[i + 1].str = NULL;
	free(tokens[i].str);
	tokens[i].str = NULL;
	return (1);
}*/

char	*handle_heredoc(t_shell *shell, t_token *tokens, char **str_heredoc)
{
	int	i;

	i = -1;
	*str_heredoc = NULL;
	while (tokens[++i].str)
		if (is_type_token(tokens[i], "HEREDOC"))
			*str_heredoc = heredoc(shell, tokens[i + 1].str);
			//process_delimiter(shell, tokens, i, str_heredoc);
	return (*str_heredoc);
}