/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:30:33 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/11 10:59:01 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_fds(int *input_fd, int pipe_fd[2], t_cmd **cmd)
{
	if (*input_fd != 0)
		close(*input_fd);
	if ((*cmd)->next)
	{
		close(pipe_fd[1]);
		*input_fd = pipe_fd[0];
	}
	else
		*input_fd = 0;
}

static void	update_state(t_cmd **cmd, pid_t *last_pid, pid_t pid)
{
	*last_pid = pid;
	*cmd = (*cmd)->next;
}

void	run_pipeline_iteration(t_cmd **cmd, int *input_fd,
		pid_t *last_pid, t_env **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	while (*cmd)
	{
		create_pipe_if_needed(*cmd, pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			break ;
		}
		else if (pid == 0)
			run_child(*cmd, *input_fd, pipe_fd, env);
		else
		{
			update_fds(input_fd, pipe_fd, cmd);
			update_state(cmd, last_pid, pid);
		}
	}
}

static void	run_pipeline_cleanup(int *input_fd, pid_t *last_pid,
			char **herdocs, int heredoc_index)
{
	if (*input_fd != 0)
		close(*input_fd);
	exit_status_set(ft_wait(last_pid));
	herdocs_clean(herdocs, heredoc_index);
}

void	run_pipeline(t_cmd *cmd, char **herdocs, t_env **env)
{
	int		input_fd;
	pid_t	last_pid;
	int		heredoc_index;

	input_fd = 0;
	last_pid = -1;
	heredoc_index = 0;
	run_pipeline_iteration(&cmd, &input_fd, &last_pid, env);
	run_pipeline_cleanup(&input_fd, &last_pid, herdocs, heredoc_index);
}
