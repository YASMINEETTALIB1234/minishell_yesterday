/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:24:13 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/04 17:29:32 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env(char **env_array)
{
	t_env	*mini;
	t_env	*cur;
	int		i;

	i = 0;
	if (!env_array || !*env_array)
	{
		mini = build_minimal_env();
		cur = mini;
		while (cur)
		{
			if (cur->value && ft_strchr(cur->value, '='))
				printf("%s\n", cur->value);
			cur = cur->next;
		}
		// free_env_list(mini);
		free(mini->key);
		free(mini->value);
		free(mini);
		return ;
	}
	while (env_array && env_array[i])
	{
		if (ft_strchr(env_array[i], '='))
			printf("%s\n", env_array[i]);
		i++;
	}
}

void	builtin_env(t_env *env)
{
	char	**arr;
	size_t	i;

	i = 0;
	arr = env_to_array(env);
	print_env(arr);
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
