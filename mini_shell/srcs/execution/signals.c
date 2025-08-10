/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:22:26 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 15:40:38 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigint(int sig)
{
	pid_t	pid;

	(void) sig;
	pid = waitpid(-1, NULL, WNOHANG);
	if (g_sig == 0)
	{
		exit_status_set(130);
		write(1, "\n", 1);
	}
	if (pid != 0)
	{
		exit_status_set(1);
		if (g_sig == 2)
		{
			g_sig = 3;
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
		}
		else
		{
			g_sig = 0;
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	handle_sigquit(int sig)
{
	(void) sig;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}

int	ft_wait(pid_t *last_pid)
{
	int		status;
	int		exit_status;
	pid_t	pid;

	exit_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == *last_pid)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(1, "Quit: 3\n", 8);
			}
		}
		pid = waitpid(-1, &status, 0);
	}
	return (exit_status);
}
