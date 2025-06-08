#include "minishell.h"

static void shell_defaults(t_shell *shell, char **env)
{ 
     duplicate_env(shell, env);
     reset_fds(shell);
     handle_signals(shell);
     shell->status.last_return = 0;
     shell->status.exit_status = 0;
     shell->status.no_exec = 0;
	shell->tokens = NULL;
}

void init_shell(t_shell *shell,char ** env)
{
     char *input_line;
     int status;
     shell_defaults(shell, env);
     while(shell->status.exit_status == 0)
     {
          shell->status.no_exec = 0;
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

          waitpid(-1, &status, 0);
          if (shell->is_parent_process == false)
               exit(0);
     }
     free_env(shell->env);
}
