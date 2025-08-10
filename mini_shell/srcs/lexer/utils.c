/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:32 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:34 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_quotes_balance(char *input)
{
    int     i;
    char    current_quote;
    
    i = 0;
    current_quote = 0;
    while (input[i])
    {
        if (is_quote(input[i]))
        {
            if (current_quote == 0)
                current_quote = input[i];
            else if (input[i] == current_quote)
                current_quote = 0;
        }
        i++;
    }
    if (current_quote != 0)
    {
        if (current_quote == '\'')
            ft_putstr_fd("minishell: unclosed single quote\n", 2);
        else
            ft_putstr_fd("minishell: unclosed double quote\n", 2);
        return (0);
    }
    return (1);
}

int get_word_length(char *str)
{
    int     len;
    char    quote;
    
    len = 0;
    quote = 0;
    while (str[len])
    {
        if (is_quote(str[len]) && quote == 0)
            quote = str[len];
        else if (str[len] == quote)
            quote = 0;
        else if (quote == 0 && (is_space(str[len]) || is_operator(str[len])))
            break;
        len++;
    }
    return (len);
}

t_token *create_token(char *value, t_token_type type)
{
    t_token *token;
    
    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = value;
    token->type = type;
    token->next = NULL;
    token->prev = NULL;
    return (token);
}

void add_token_to_list(t_token **head, t_token **tail, t_token *new_token)
{
    if (!new_token || !head || !tail)
        return;
    if (!*head)
    {
        *head = new_token;
        *tail = new_token;
    }
    else
    {
        (*tail)->next = new_token;
        new_token->prev = *tail;
        *tail = new_token;
    }
}
