#include "../../includes/minishell.h"

static int	ft_count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	ft_fill_env_array(t_env **env_array, t_env *env, int count)
{
	int	i;

	i = 0;
	while (env && i < count)
	{
		env_array[i] = env;
		env = env->next;
		i++;
	}
}

static void	ft_sort_env_array(t_env **env_array, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(env_array[i]->key, env_array[j]->key) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	ft_print_env_array(t_env **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (env_array[i]->value && *(env_array[i]->value))
			printf("declare -x %s=\"%s\"\n", env_array[i]->key,
				env_array[i]->value);
		else
			printf("declare -x %s\n", env_array[i]->key);
		i++;
	}
}

int	print_sorted_env(t_env *env)
{
	t_env   **env_array;
	int     count;

	count = ft_count_env_vars(env);
	if (count == 0)
		return (0);
	env_array = malloc(sizeof(t_env *) * count);
	if (!env_array)
		return (1);
	ft_fill_env_array(env_array, env, count);
	ft_sort_env_array(env_array, count);
	ft_print_env_array(env_array, count);
	free(env_array);
    return (0);
}