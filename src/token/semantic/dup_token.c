#include "minishell.h"

void	dup_tokens(t_shell *shell, t_token *tokens)
{
	int	i;
    
    if (has_heredoc(shell, tokens) == false)
    {
        i = 0;
        shell->tokens = (t_token *)malloc(sizeof(t_token) * (shell->tokens_size + 1));
        while (tokens[i].str)
        {
            shell->tokens[i].str = ft_strdup(tokens[i].str);
            shell->tokens[i].type = tokens[i].type;
            i++;
        }
        shell->tokens[i].str = NULL;
    }
}