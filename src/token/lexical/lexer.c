#include "minishell.h"

static int count_next_token(char *input, int *index_inp)
{
	int found_token = 0, in_quotes = 0;

	while (input[*index_inp])
	{
		toggle_quote_state(&in_quotes, input, index_inp);
		if (!in_quotes)
		{
			if (input[*index_inp] == ' ' || is_separator(input[*index_inp]))
				return found_token;
		}
		found_token = 1;
		(*index_inp)++;
	}
	return found_token;
}

static int count_tokens(char *line)
{
	int index_line = 0, count = 0;

	while (line[index_line])
	{
		skip_whitespace(line, &index_line);
		if (line[index_line])
		{
			if (is_separator(line[index_line]))
			{
				index_line++;
				count++;
			}
			count += count_next_token(line, &index_line);
		}
	}
	return count;
}


void gettokens(t_shell *shell, char *input_line, t_token **tokens)
{
    int numb_tokens;

    numb_tokens = count_tokens(input_line);
	shell->tokens_size = numb_tokens;
    if (numb_tokens == 0)
        return;

    *tokens = (t_token *)malloc(sizeof(t_token) * (numb_tokens + 1));
    if (*tokens == NULL)
    {
        perror("Erro: Falha ao alocar memória para tokens");
        shell->status.last_return = 1;
        return;
    }
    process_tokens(shell, input_line, *tokens, numb_tokens);
	reorganize_tokens(*tokens);
}