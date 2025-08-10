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
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
        return (0);
    }
    if (tokens->type == TOKEN_PIPE && tokens->prev && tokens->prev->type == TOKEN_PIPE)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
        return (0);
    }
    return (1);
}

static int check_redir_errors(t_token *tokens)
{
    if (tokens->type >= TOKEN_REDIR_IN && tokens->type <= TOKEN_HEREDOC)
    {
        if (!tokens->next || tokens->next->type != TOKEN_WORD)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
            return (0);
        }
    }
    return (1);
}

static int check_pipe_end(t_token *tokens)
{
    if (tokens && tokens->prev && tokens->prev->type == TOKEN_PIPE)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
        return (0);
    }
    return (1);
}

int validate_syntax(t_token *tokens)
{
    while (tokens && tokens->type != TOKEN_END)
    {
        if (!check_pipe_errors(tokens))
            return (exit_status_set(258), 0);
        if (!check_redir_errors(tokens))
            return (exit_status_set(258), 0);
        tokens = tokens->next;
    }
    if (!check_pipe_end(tokens))
        return (exit_status_set(258), 0);
    return (1);
}