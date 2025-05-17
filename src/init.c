#include "minishell.h"

static void shell_defaults(t_shell *shell, char **env)
{
     reset_fds(shell);
     duplicate_env(shell, env);
     shell->status.last_return = 0;
     shell->status.exit_status = 0;
     shell->status.no_exec = 0;
     handle_signals();
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
          {
               free(input_line);
               break;
          }
          token_analysis(shell, input_line);
		reset_std(shell); 
		close_fds(shell);
		reset_fds(shell);
          waitpid(-1, &shell->status.exit_status, 0);
     }
     free_env(shell->env);
}