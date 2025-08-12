/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:48:18 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/12 13:23:49 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	delete_env_variable(char *key, t_env **env)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr->kv);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env->kv);
		free(env);
		env = tmp;
	}
}

t_env	*create_env_node(char *kv)
{
	char	*eq;
	t_env	*n;

	eq = ft_strchr(kv, '=');
	n = malloc(sizeof(t_env));
	if (!n || !eq)
		return (NULL);
	n->kv = ft_strdup_safe(kv);
	n->key = ft_strndup(kv, eq - kv);
	n->value = ft_strdup_safe(eq + 1);
	n->next = NULL;
	return (n);
}
