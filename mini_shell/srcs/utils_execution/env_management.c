/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:41:56 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:41:58 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	get_equal_index(const char *str)
{
	size_t	i = 0;

	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

t_env	*create_node(char *str, int add_equal)
{
	t_env	*node;
	size_t	eq_idx;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	eq_idx = get_equal_index(str);
	node->kv = ft_strdup(str);
	if (str[eq_idx] == '=')
    {
        node->key = ft_substr(str, 0, eq_idx + add_equal);
        node->value = ft_strdup(str + eq_idx + 1);
    }
	else
    {
        node->key = ft_substr(str, 0, eq_idx);
        node->value = ft_strdup("");
    }
	node->next = NULL; 
	return (node);
}

t_env	*copy_environment(char **env)
{
	t_env	*head = NULL;
	t_env	*last = NULL;
	t_env	*new;

	if (!env || !*env)
		return (build_minimal_env());
	while (env && *env)
	{
		new = create_node(*env, 0);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
		env++;
	}
	return (head);
}

void	free_environment(t_env **env)
{
	t_env	*next;

	while (*env)
	{
		next = (*env)->next;
		free((*env)->key);
		free((*env)->value);
		free(*env);
		*env = next;
	}
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
