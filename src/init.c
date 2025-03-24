#include "minishell.h"

static void shell_defaults(t_shell *shell, char **env)
{
     duplicate_env(shell, env);
}

void init_shell(t_shell *shell,char ** env)
{  
     
     shell_defaults(shell, env);
     char *input_line;

     while(shell->status.exit_status == 0)
     {
          shell->is_parent_process = true;
          shell->tokens = NULL;
          if (!ft_readline(shell, &input_line))
               break ;
          token_analysis(shell, input_line);
     }
}