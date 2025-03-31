#include "minishell.h"

void	process_separator(char *line, int *index_line, t_token *tokens, int *index_tok)
{
	char	*separator;

	if (line[*index_line] == '>' || line[*index_line] == '<' || line[*index_line] == '|')
	{
		separator = get_separator(line, index_line);
		if (separator != NULL && separator[0])
		{
			tokens[*index_tok].str = separator;
			tokens[*index_tok].type = type_token(separator, 0);
			(*index_tok)++;
		}
	}
}

void	process_tokens(t_shell *shell, char *line, t_token *tokens, int numb_tokens)
{
	int		index_line;
	int		index_tok;
	char	*current_str;
	int		in_quotes;

	index_line = 0;
	index_tok = 0;
	while (line[index_line] && index_tok < numb_tokens)
	{
		skip_whitespace(line, &index_line);
		if (is_separator(line[index_line]))
		{
			process_separator(line, &index_line, tokens, &index_tok);
			continue ;
		}
		current_str = extract_token(line, &index_line, &in_quotes);
		if (current_str)
		{
			current_str = process_expansion(shell, current_str, 0);
			tokens[index_tok].str = ft_strdup(current_str);
			tokens[index_tok].type = type_token(current_str, in_quotes);
			index_tok++;
		}
	} 
	tokens[index_tok].str = NULL;
}
