#include "../includes/minishell.h"

static t_env	*ft_create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
    if (!value)
        new_node->value = ft_strdup("");
    else
    {
        new_node->value = ft_strdup(value);
    }
	new_node->kv = join_kv(key, value);
	if (!new_node->kv || !new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	ft_lstadd_back(t_env **env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = ft_create_env_node(key, value);
	if (!new_node)
		return ;
	if (!*env)
		*env = new_node;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}