#include "minishell.h"

void	process_separator(char *line, int *index_line, t_token *tokens, int *index_tok)
{
	char	*separator;

	if (line[*index_line] == '>' || line[*index_line] == '<' || line[*index_line] == '|')
	{
		separator = get_separator(line, index_line);
		if (separator != NULL && separator[0])
		{
			if (*index_tok > 0 && tokens[*index_tok - 1].str && is_type_token(tokens[*index_tok - 1], "PIPE"))
			{
				tokens[*index_tok].str = ft_strdup("echo");
				tokens[*index_tok].type = "ARGUMENT";
				(*index_tok)++;
			}
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

	index_line = 0;
	index_tok = 0;
	shell->tmp.active_quote = 0;
	while (line[index_line] && index_tok < numb_tokens)
	{
		skip_whitespace(line, &index_line);
		current_str = extract_token(line, &index_line, &shell->tmp.active_quote);
		if (current_str)
		{
			if (is_separator(current_str[0]) && current_str[1] == '\0')
			{
				process_separator(line, &index_line, tokens, &index_tok);
				free(current_str);
			}
			else
			{
				current_str = process_expansion(shell, current_str, 0);
				tokens[index_tok].str = current_str;
				tokens[index_tok].type = type_token(current_str, 1);
				index_tok++;
			}
		}
	}
	tokens[index_tok].str = NULL;
	index_tok = 0;
	while (index_tok < numb_tokens)
	{
		printf("%d\t%s\t%s\n", index_tok, tokens[index_tok].str, tokens[index_tok].type);
		index_tok++;
	}
}

/*void	process_tokens(t_shell *shell, char *line, t_token *tokens, int numb_tokens)
{
	int		index_line;
	int		index_tok;
	char	*current_str;

	index_line = 0;
	index_tok = 0;
	while (line[index_line] && index_tok < numb_tokens)
	{
		skip_whitespace(line, &index_line);
		current_str = extract_token(line, &index_line);
		if (current_str == NULL)
		{
			process_separator(line, &index_line, tokens, &index_tok);
			continue ;
		}
		printf("Token: %s\n", current_str);
		current_str = process_expansion(shell, current_str, 0);
		printf("Token after: %s\n", current_str);
		if (current_str)
		{
			printf("Entrei: %s\n", current_str);
			tokens[index_tok].str = current_str;
			tokens[index_tok].type = current_str;
			index_tok++;
		}
		skip_whitespace(line, &index_line);
		process_separator(line, &index_line, tokens, &index_tok);
		printf("Token: %s\n", tokens[index_tok].str);
		//printf("Tipo: %s\n", tokens[index_tok].type);
		//free_str_and_set_null(&current_str);
	}
	tokens[index_tok].str = NULL;
}*/