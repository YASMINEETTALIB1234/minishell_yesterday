/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:29:13 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 15:42:52 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_command(t_cmd *cmd_head, char **herdocs,
		t_env **env, int herdocs_count)
{
	if (!cmd_head)
		return ;
	if (herdocs && herdocs_prepare(cmd_head, herdocs))
	{
		herdocs_clean(herdocs, herdocs_count);
		return ;
	}
	if (is_single_builtin(cmd_head))
	{
		run_single_builtin(cmd_head, env);
		return ;
	}
	run_pipeline(cmd_head, herdocs, env);
}
