#include "minishell.h"

/*static char	**cmd_tab(t_token *tokens)
{
	int		i;
	int		count = 0;
	char	**argv;

	// contar quantos tokens antes do redirecionador
	while (tokens[count].str && !is_type_token(tokens[count], "REDIR_OUT"))
		count++;

	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);

	i = 0;
	while (i < count)
	{
		argv[i] = ft_strdup(tokens[i].str); // duplica os argumentos
		i++;
	}
	argv[i] = NULL; // termina com NULL
	return (argv);
}

static void	execute_command_test(t_shell *shell, t_token *tokens)
{
	pid_t	pid;
	char	**argv;
	int		status;

	argv = cmd_tab(tokens); // função que vamos criar já já

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		shell->status.last_return = 1;
		return;
	}
	if (pid == 0)
	{
		// processo filho executa o comando
		execvp(argv[0], argv); // ou execve se você quiser usar envp
		perror("minishell: exec");
		exit(1); // se falhar
	}
	else
	{
		// processo pai espera
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->status.last_return = WEXITSTATUS(status);
	}
	free_matrix(argv);
}

static void	redir(t_shell *shell, char *file, char *type)
{
	if (shell->fd_out > 0)
		close(shell->fd_out);
	if (ft_strcmp(type, "trunc") == 0)
		shell->fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	else
		shell->fd_out = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	if (shell->fd_out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": No such file or directory", 2);
		shell->status.last_return = 1;
		shell->status.no_exec = 1;
		return ;
	}
	dup2(shell->fd_out, 2);
}


static int redirection(t_shell *shell, char *file, char *type)
{
    if (!file || !type)
		return (-1);
	if (shell->fd_out > 0)
        close(shell->fd_out);
    if (ft_strcmp(type, "trunc") == 0)
		shell->fd_out = open(file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    else
	    shell->fd_out = open(file, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (shell->fd_out == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(file, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        shell->status.last_return = 1;
        shell->status.no_exec = 1;
        return (-1);
    }
    if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
    {
        ft_putstr_fd("minishell: error: failed to redirect output\n", STDERR_FILENO);
        close(shell->fd_out);
        return (-1);
    }
    return (0);
}

void	handle_redirection_test(t_shell *shell)
{
	int	i = 0;

	while (shell->tokens[i].str)
	{
		if (is_type_token(shell->tokens[i], "REDIR_OUT"))
		{
			if (!shell->tokens[i + 1].str)
			{
				ft_putendl_fd("minishell: syntax error: expected filename after '>'", 2);
                shell->status.last_return = 258;
                return;
			}
			redir(shell, shell->tokens[i + 1].str, "trunc");
			i += 2;
		}
		else
			i++;
	}
	execute_command_test(shell, shell->tokens);
}
*/
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
	shell->charge = 0;
	//handle_redirection_test(shell);
	handle_redirection(shell, 0, 0);
	execute_command(shell, 0, 0);
    ft_free_tokens(tokens);
    ft_free(input_line);
}