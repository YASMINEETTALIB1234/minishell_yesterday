/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:35:03 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 16:10:39 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_env(t_env **env, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*new_node;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, (char *)key) == 0)
		{
			free(cur->value);
			free(cur->kv);
			cur->value = ft_strdup_safe(value);
			cur->kv = join_kv(key, value);
			return ;
		}
		cur = cur->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup_safe(key);
	new_node->value = ft_strdup_safe(value);
	new_node->kv = join_kv(key, value);
	new_node->next = *env;
	*env = new_node;
}

static size_t	count_valid_env(t_env *env)
{
	size_t	count;

	count = 0;
	while (env)
	{
		if (env->kv && ft_strchr(env->kv, '='))
			count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_array(t_env *env)
{
	size_t	n;
	size_t	i;
	t_env	*c;
	char	**arr;

	n = count_valid_env(env);
	arr = malloc((n + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	c = env;
	while (c)
	{
		if (c->kv && ft_strchr(c->kv, '='))
			arr[i++] = ft_strdup_safe(c->kv);
		c = c->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_env	*build_minimal_env(void)
{
	char	*cwd;
	t_env	*head;

	cwd = getcwd(NULL, 0);
	head = create_env_node("_=/usr/bin/env");
	if (!cwd)
		return (NULL);
	if (!head)
		return (free(cwd), NULL);
	set_env(&head, "PATH", "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	set_env(&head, "SHLVL", "1");
	set_env(&head, "PWD", cwd);
	free(cwd);
	return (head);
}
