/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:32:00 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 15:12:59 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	setup_child_pipes(int input_fd, t_cmd *cmd, int pipe_fd[2])
{
	if (input_fd != 0)
		dup2(input_fd, STDIN_FILENO);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	handle_child_redirections(t_cmd *cmd)
{
	if (cmd->redirs)
	{
		if (handle_redirections(cmd) == -1)
		{
			exit_status_set(1);
			exit(EXIT_FAILURE);
		}
	}
}

static void	execute_child_command(t_cmd *cmd, t_env **env)
{
	if (cmd->args)
	{
		if (check_builtin(cmd->args[0]))
		{
			exit_status_set(0);
			exit(execute_builtin(cmd->args, env));
		}
		execute_external(cmd, env);
	}
	exit(0);
}

void	run_child(t_cmd *cmd, int input_fd, int pipe_fd[2], t_env **env)
{
	setup_child_signals();
	setup_child_pipes(input_fd, cmd, pipe_fd);
	handle_child_redirections(cmd);
	execute_child_command(cmd, env);
}
