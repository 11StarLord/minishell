#include "minishell.h"

bool is_separator(char c)
{
    if (c == '|' || c == '>' || c == '<')
        return (true);
    else
        return (false);
}

void	skip_whitespace(char *line, int *i)
{
    while (line[*i] == ' ' || line[*i] == '\t')
        (*i)++;
}

char    *type_token(char *str, int in_quotes)
{
    if (in_quotes == 1)
        return ("ARGUMENT");
    else if (ft_strcmp(str, ">") == 0)
        return ("REDIR_OUT");
    else if (ft_strcmp(str, "<") == 0)
        return ("REDIR_IN");
    else if (ft_strcmp(str, ">>") == 0)
        return ("APPEND");
    else if (ft_strcmp(str, "<<") == 0)
        return ("HEREDOC");
    else if (ft_strcmp(str, "|") == 0)
        return ("PIPE");
    return ("ARGUMENT");
}

char	*get_separator(char *line, int *index_line)
{
	int	length_separator;
	char	*separator;

	length_separator = 1;
	if (line[*index_line] == line[*index_line + 1])
		length_separator = 2;
	separator = ft_strndup(line + *index_line, length_separator);
	*index_line += length_separator;
	return (separator);
}

bool	is_type_token(t_token token, char *type)
{
	if (token.str && token.str[0] && ft_strcmp(token.type, type) == 0)
		return (true);
	return (false);
}