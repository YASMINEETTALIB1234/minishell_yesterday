/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_target.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:06 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:08 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_string(t_redir *redir, char *target, global_struct *global_struct)
{
	char	*result;
	char	*var_expansion;
	int		i;
	int		quote_state;
	int last_index;
	if (!target)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	quote_state = NO_QUOTE;
	while (target[i])
	{
		if ((target[i] == '\'' || target[i] == '"'))
		{
			if (target[i] != get_quote_char(quote_state) && quote_state != NO_QUOTE)
				result = ft_strjoin_char(result, target[i], 0);
			update_quote_state(target[i], &quote_state);
			i++;
		}
		else if (target[i] == '$' && quote_state != SINGLE_QUOTE)
		{
			var_expansion = expand_variable(target, &i, global_struct->env,
											*exit_status_get());
			if (var_expansion)
			{
				last_index = ft_strlen(result) - 1;
				if (last_index < 0)
					last_index = 0;
				if (quote_state == NO_QUOTE && count_words(result[last_index],var_expansion, target[i]) != 1)
					redir->ambiguous_flag = 1;
				result = ft_strjoin(result, var_expansion, 0);
				free(var_expansion);
			}
		}
		else
		{
			result = ft_strjoin_char(result, target[i], 0);
			i++;
		}
		if (!result)
			return (NULL);
	}
	return (result);
}

void	expand_redir_target(t_redir *redir, global_struct *global_struct)
{
	char	*expanded;

	if (!redir || !redir->target || !global_struct)
		return ;
	expanded = expand_string(redir, redir->target, global_struct);
	if (!expanded)
	{
		redir->ambiguous_flag = 1;
		return ;
	}
	free(redir->target);
	redir->target = expanded;
}
