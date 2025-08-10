/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:46 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:48 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_redir *create_redirection(t_token_type type, char *target)
{
    t_redir *redir;
    
    redir = malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    redir->type = type;
    redir->target = ft_strdup(target);
    redir->file = NULL; // Initialize file for heredoc
    redir->fd = -1;
    redir->ambiguous_flag = 0;
    redir->should_expand = 1;
    if (!redir->target)
    {
        free(redir);
        return (NULL);
    }
    redir->next = NULL;
    return (redir);
}

void    add_redirection_to_cmd(t_cmd *cmd, t_redir *redir)
{
    t_redir *current;
    
    if (!cmd || !redir)
        return;
    if (!cmd->redirs)
        cmd->redirs = redir;
    else
    {
        current = cmd->redirs;
        while (current->next)
            current = current->next;
        current->next = redir;
    }
}

t_cmd	*init_command_parsing(global_struct *global_struct)
{
    t_cmd   *cmd;
    
    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->line = NULL;
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->env = global_struct->env;
    cmd->next = NULL;
    return (cmd);
}
