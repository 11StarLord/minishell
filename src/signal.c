#include "minishell.h"

static int	g_signal = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (g_signal == 1)
		rl_redisplay();
}

void	handle_signals()
{
	g_signal = 1;
	signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}