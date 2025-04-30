#include "minishell.h"

static void shell_defaults(t_shell *shell, char **env)
{
     shell->stdin = dup(STDIN_FILENO);
     shell->stdout = dup(STDOUT_FILENO);
     duplicate_env(shell, env);
     reset_fds(shell);
}

void init_shell(t_shell *shell,char ** env)
{  
     char *input_line;
     
     shell_defaults(shell, env);
     while(shell->status.exit_status == 0)
     {
          shell->is_parent_process = true;
          shell->stdin = dup(STDIN_FILENO);
          shell->stdout = dup(STDOUT_FILENO);
          shell->tokens = NULL;
          if (!ft_readline(shell, &input_line))
               break ;
          token_analysis(shell, input_line);
		reset_std(shell);
		close_fds(shell);
		reset_fds(shell);
     }
}