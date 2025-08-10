/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rederection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:22:01 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 20:50:59 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redirection(char *file, int ambiguous_flag)
{
	int	fd;

	if (ambiguous_flag == 1)
		return (ft_putstr_fd(AMBIGUOUS_MESSAGE, 2), -1);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd(TARGET_NOT_FOUND, 2), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redirection(char *file, int ambiguous_flag)
{
	int	fd;

	if (ambiguous_flag == 1)
		return (ft_putstr_fd(AMBIGUOUS_MESSAGE, 2), -1);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_append_redirection(char *file, int ambiguous_flag)
{
	int	fd;

	if (ambiguous_flag == 1)
		return (ft_putstr_fd(AMBIGUOUS_MESSAGE, 2), -1);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (ft_putstr_fd(TARGET_NOT_FOUND, 2), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	int		result;
	t_redir	*r;

	r = cmd->redirs;
	result = 0;
	while (r && !result)
	{
		if (r->type == TOKEN_REDIR_IN)
			result = handle_input_redirection(r->target, r->ambiguous_flag);
		else if (r->type == TOKEN_REDIR_OUT)
			result = handle_output_redirection(r->target, r->ambiguous_flag);
		else if (r->type == TOKEN_APPEND)
			result = handle_append_redirection(r->target, r->ambiguous_flag);
		else if (r->type == TOKEN_HEREDOC)
			result = handle_input_redirection(r->file, r->ambiguous_flag);
		if (result == -1)
			return (-1);
		r = r->next;
	}
	return (0);
}
