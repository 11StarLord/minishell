#include "minishell.h"

void	token_analysis(t_shell *shell, char *input_line)
{
	t_token	*tokens;

	input_line = ft_strtrim(input_line, " ");
	if (!input_line || !input_line[0])
	{
		ft_free(input_line);
		return ;
	}
	gettokens(shell, input_line,&tokens);
	if (!tokens)
	{
		ft_free(input_line);
		shell->status.last_return = 1;
		return ;
	}

	int index_tok = 0;
 	while (tokens[index_tok].str)
 	{
 		printf("%d\t%s\t%s\n", index_tok, tokens[index_tok].str,tokens[index_tok].type);
		 index_tok++;
 	}
	/*process_and_validate_line(shell, &tokens);
	if (shell->status.last_return == 258)
	{
		ft_free(input_line);
		return ;
	}
	shell->charge = 1;
	redir_and_exec(shell, 0, 0);*/
	ft_free_tokens(tokens);
	ft_free(input_line);
}