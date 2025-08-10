/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:17 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:19 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_quotes_type(char *str)
{
	int		i;
	char	first_quote;

	i = 0;
	first_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !first_quote)
		{
			first_quote = str[i];
			if (first_quote == '\'')
				return (SINGLE_QUOTE);
			else
				return (DOUBLE_QUOTE);
		}
		i++;
	}
	return (NO_QUOTE);
}

static int	count_clean_chars(char *target)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (target[i])
	{
		if (!quote && (target[i] == '\'' || target[i] == '"'))
			quote = target[i];
		else if (quote && target[i] == quote)
			quote = 0;
		else
			len++;
		i++;
	}
	return (len);
}

char	*remove_quotes(char *target)
{
	char	*clean;
	int		i;
	int		j;
	char	quote;

	clean = malloc(sizeof(char) * (count_clean_chars(target) + 1));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (target[i])
	{
		if (!quote && (target[i] == '\'' || target[i] == '"'))
			quote = target[i];
		else if (quote && target[i] == quote)
			quote = 0;
		else
			clean[j++] = target[i];
		i++;
	}
	return (clean[j] = 0, clean);
}

void	update_quote_state(char c, int *quote_state)
{
	if (c == '\'' && *quote_state != DOUBLE_QUOTE)
	{
		if (*quote_state == SINGLE_QUOTE)
			*quote_state = NO_QUOTE;
		else
			*quote_state = SINGLE_QUOTE;
	}
	else if (c == '"' && *quote_state != SINGLE_QUOTE)
	{
		if (*quote_state == DOUBLE_QUOTE)
			*quote_state = NO_QUOTE;
		else
			*quote_state = DOUBLE_QUOTE;
	}
}

char get_quote_char(int macro)
{
	if (macro == SINGLE_QUOTE)
		return '\'';
	else if (macro == DOUBLE_QUOTE)
		return '"';
	return 0;
}
