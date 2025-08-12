/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:42 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:44 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int check_pipe_errors(t_token *tokens)
{
    if (tokens->type == TOKEN_PIPE && tokens->prev == NULL)
    {
        ft_putstr_fd(PIPE_ERROR, 2);
        return (0);
    }
    if (tokens->type == TOKEN_PIPE && tokens->prev && tokens->prev->type != TOKEN_WORD)
    {
        ft_putstr_fd(PIPE_ERROR, 2);
        return (0);
    }
    return (1);
}

static int check_redir_errors(t_token *tokens)
{
    if (tokens->type >= TOKEN_REDIR_IN && tokens->type <= TOKEN_HEREDOC)
    {
        if (!tokens->next)
        {
            ft_putstr_fd(REDIRECTION_ERROR, 2);
            return (0);
        }
        if (tokens->next->type != TOKEN_WORD)
        {
            if (tokens->next->type == TOKEN_PIPE)
                ft_putstr_fd(PIPE_ERROR, 2);
            else
                ft_putstr_fd(REDIRECTION_ERROR, 2);
            return (0);
        }
    }
    return (1);
}

static int check_pipe_end(t_token *tokens)
{
    if (tokens && tokens->type == TOKEN_PIPE)
    {
        ft_putstr_fd(PIPE_ERROR, 2);
        return (0);
    }
    return (1);
}

int validate_syntax(t_token *tokens)
{
    t_token *last;

    while (tokens)
    {
        if (!check_pipe_errors(tokens))
            return (exit_status_set(258), 0);
        else if (!check_redir_errors(tokens))
            return (exit_status_set(258), 0);
        last = tokens;
        tokens = tokens->next;
    }
    if (!check_pipe_end(last))
        return (exit_status_set(258), 0);
    return (1);
}