#include "minishell.h"

void	process_command(t_shell *shell)
{
	int	pos_token;

	pos_token = 0;
	handle_redirection(shell, 0);
	handle_execution(shell, &pos_token);
}

static	int	count_commands(t_token *tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (tokens[i].str)
	{
		if (is_type_token(tokens[i], "PIPE"))
			count++;
		i++;
	}
	return (count);
}

static void	tokens_to_command(t_shell *shell, t_token *tokens)
{
	 int	i;
	 int	cmd_index;
	 int	arg_index;
	 char	**args;

	 i = 0;
	 cmd_index = 0;
	 shell->num_commands = count_commands(tokens);
	 shell->commands = (t_command *)malloc(sizeof(t_command) * (shell->num_commands + 1));
	 if (!shell->commands)
	 	return;
	 while (cmd_index < shell->num_commands)
	 {
		args = malloc(sizeof(char *) * (shell->tokens_size + 1));
		arg_index = 0;
		if (is_type_token(tokens[i], "ARGUMENT") && !is_type_token(tokens[i + 1], "ARGUMENT"))
		{
			shell->commands[j++].args = ft_strdup(tokens[i].str);
		}
		cmd_index++;
	 }
	 shell->commands[cmd_index].args = NULL;
}

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
	if(is_valid_redirect_syntax(tokens, shell) == false)
	{
		ft_free(input_line);
		ft_free_tokens(tokens);
		return ;
	}
	dup_tokens(shell,tokens);
	tokens_to_command(shell, tokens);
	int	i = 0;
	while (shell->commands[i].args)
	 {
			printf("%s \n", shell->commands[i].args);
		i++;
	 }
	/*shell->charge = 1;
	process_command(shell);*/
    ft_free_tokens(tokens);
    ft_free(input_line);
}