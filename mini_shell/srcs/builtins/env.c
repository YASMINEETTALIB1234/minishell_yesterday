/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:24:13 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/11 10:44:23 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_minimal_env(void)
{
	t_env	*mini;
	t_env	*cur;

	mini = build_minimal_env();
	cur = mini;
	while (cur)
	{
		if (cur->value && ft_strchr(cur->value, '='))
			printf("%s\n", cur->value);
		cur = cur->next;
	}
	free(mini->key);
	free(mini->value);
	free(mini);
}

static void	print_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array && env_array[i])
	{
		if (ft_strchr(env_array[i], '='))
			printf("%s\n", env_array[i]);
		i++;
	}
}

void	print_env(char **env_array)
{
	if (!env_array || !*env_array)
	{
		print_minimal_env();
		return ;
	}
	print_env_array(env_array);
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
