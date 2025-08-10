/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:08:58 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/09 19:17:17 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_heredocs(t_token *tokens)
{
	int	count;

	count = 0;
	if (tokens == NULL)
		return (0);
	while (tokens->type != TOKEN_END)
	{
		if (tokens->type == TOKEN_HEREDOC)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

// void	process_heredocs_for_command(t_cmd *cmd, char **heredoc, int *index)
// {
// 	t_redir	*redir;
// 	char	*file;

// 	redir = cmd->redirs;
// 	while (redir)
// 	{
// 		if (redir->type == TOKEN_HEREDOC)
// 		{
// 			file = handle_heredoc_input(redir, heredoc, *index);
// 			if (!file)
// 				return ;
// 			redir->file = file;
// 			(*index)++;
// 		}
// 		redir = redir->next;
// 	}
// }

static int process_heredocs_for_command(t_cmd *cmd, char **heredoc, int *index)
{
	t_redir	*redir;
	char	*file;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			file = handle_heredoc_input(redir, heredoc, cmd->env, *index);
			if (!file)
				return (1);
			redir->file = file;
			(*index)++;
		}
		redir = redir->next;
	}
	return (0);
}


// void	herdocs_prepare(t_cmd *cmd_list, char **heredoc)
// {
// 	int		heredoc_index;

// 	heredoc_index = 0;
// 	while (cmd_list)
// 	{
// 		process_heredocs_for_command(cmd_list, heredoc, &heredoc_index);
// 		cmd_list = cmd_list->next;
// 	}
// }

int herdocs_prepare(t_cmd *cmd_list, char **heredoc)
{
	int		heredoc_index;
	int		status;

	heredoc_index = 0;
	while (cmd_list)
	{
		status = process_heredocs_for_command(cmd_list, heredoc, &heredoc_index);
		if (status == 1)
			return (1);
		cmd_list = cmd_list->next;
	}
	return (0);
}


void	herdocs_clean(char **heredocs, int count)
{
	int	i;

	i = 0;
	if (!heredocs)
		return ;
	while (i < count)
	{
		if (heredocs[i])
		{
			unlink(heredocs[i]);
			heredocs[i] = NULL;
		}
		i++;
	}
}
