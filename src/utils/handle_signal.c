#include "minishell.h"

int g_signal = 0;

void	sigint_handler(int sig)
{	
	g_signal = 1;
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();


}
 
void	handle_signals(t_shell *shell)
{	
	(void)shell;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);

	if (g_signal == 1)
	{
		shell->status.last_return = 130;
	}
}

void	reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

