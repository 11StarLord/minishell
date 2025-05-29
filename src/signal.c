#include "minishell.h"

static t_signal	g_signal;

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (g_signal.g_redisplay == 1)
		rl_redisplay();
	if (g_signal.g_temp_shell)
	{
		g_signal.g_temp_shell->last_return = 130;
	}
}

void	handle_signals(t_shell *shell)
{
	struct sigaction sa;
	g_signal.g_temp_shell = shell;
	g_signal.g_redisplay = 1;
	sigaction(SIGINT, sigint_handler);
	sigaction(SIGQUIT, SIG_IGN);
}

void	sigquit(int sig)
{
	(void)sig;
}