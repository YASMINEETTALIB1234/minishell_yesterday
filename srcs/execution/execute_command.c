/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:29:13 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/12 17:26:09 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	create_pipe_if_needed(t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit_status_set(1);
		exit(EXIT_FAILURE);
	}
}

void	execute_command(t_cmd *cmd_head, char **herdocs,
		t_env **env, int herdocs_count)
{
	if (!cmd_head || !herdocs)
		return ;
	if (herdocs_prepare(cmd_head, herdocs))
	{
		herdocs_clean(herdocs, herdocs_count);
		free_args(herdocs, herdocs_count);
		return ;
	}
	if (is_single_builtin(cmd_head))
	{
		run_single_builtin(cmd_head, env);
		herdocs_clean(herdocs, herdocs_count);
		free_args(herdocs, herdocs_count);
		return ;
	}
	run_pipeline(cmd_head, herdocs, env, herdocs_count);
}
