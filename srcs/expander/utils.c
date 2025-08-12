/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:22 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:24 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_var_name(char *str, int *len)
{
	char	*var_name;
	int		i;

	*len = 0;
	if (!str || !is_valid_var_start(str[0]))
		return (NULL);
	while (str[*len] && is_valid_var_char(str[*len]))
		(*len)++;
	var_name = malloc(sizeof(char) * (*len + 1)); // testi had lblassa
	if (!var_name)
		return (NULL);
	i = -1;
	while (++i < *len)
		var_name[i] = str[i];
	var_name[i] = '\0';
	return (var_name);
}

char	*expand_variable(char *str, int *i, t_env *env_list)
{
	char	*var_value;
	char	*var_name;
	int		var_len;

	if (str[*i + 1] == '?')
		return (*i += 2, ft_itoa(*exit_status_get()));
	var_name = extract_var_name(str + *i + 1, &var_len);
	if (!var_name)
		return ((*i)++, ft_strdup("$"));
	if (!var_name)
		return (NULL);
	*i += var_len + 1;
	var_value = get_env_value(env_list, var_name);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}
