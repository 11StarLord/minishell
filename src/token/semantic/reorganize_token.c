#include "minishell.h"

static int is_redirection_token(t_token token) {
    return (!strcmp(token.type, "REDIR_OUT") ||
            !strcmp(token.type, "REDIR_IN") ||
            !strcmp(token.type, "APPEND") ||
            !strcmp(token.type, "HEREDOC"));
}

void reorganize_tokens(t_token *tokens) {
   
	int		index_tok;
	t_token	tmp_token[3];

	index_tok = -1;
	while (tokens[++index_tok].str)
	{
		if (is_redirection_token(tokens[index_tok]))
		{
			if (tokens[index_tok + 1].str == NULL || tokens[index_tok + 2].str == NULL)
				break ;
			if (tokens[index_tok + 2].str
				&& is_type_token(tokens[index_tok + 2], "ARGUMENT"))
			{
				tmp_token[0] = tokens[index_tok];
				tmp_token[1] = tokens[index_tok + 1];
				tmp_token[2] = tokens[index_tok + 2];
				tokens[index_tok] = tmp_token[2];
				tokens[index_tok + 1] = tmp_token[0];
				tokens[index_tok + 2] = tmp_token[1];
				index_tok = 0;
			}
		}
	}
}