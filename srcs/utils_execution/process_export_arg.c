#include "../../includes/minishell.h"

static int	ft_is_valid_identifier(char *arg)
{
	int	i;

	if (!is_valid_var_start(arg[0]))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
	{
		if (!is_valid_var_char(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_parse_export_arg(char *arg, char **key, char **value, int *append)
{
	char	*plus_pos;
	char	*eq_pos;

	*append = 0;
	plus_pos = ft_strstr(arg, "+=");
	if (plus_pos)
	{
		*append = 1;
		*key = ft_strndup(arg, plus_pos - arg);
		*value = ft_strdup(plus_pos + 2);
		return (ft_is_valid_identifier(*key));
	}
	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
	{
		*key = ft_strndup(arg, eq_pos - arg);
		*value = ft_strdup(eq_pos + 1);
		return (ft_is_valid_identifier(*key));
	}
	*key = ft_strdup(arg);
	*value = NULL;
	return (ft_is_valid_identifier(*key));
}

static char	*ft_create_new_value(char *old_value, char *value, int is_append)
{
	if (is_append && old_value && *old_value)
		return (ft_strjoin(old_value, value, '\0'));
	return (free(old_value), ft_strdup(value));
}

static void	ft_update_existing_var(t_env *existing, char *key, char *value,
		int is_append)
{
	if (value)
	{
		existing->value = ft_create_new_value(existing->value, value, is_append);
        free(existing->kv);
		existing->kv = join_kv(key, existing->value);
	}
}

static t_env	*ft_find_env_var(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	ft_update_env_var(t_env **env, char *key, char *value, int append)
{
	t_env	*existing;

	existing = ft_find_env_var(*env, key);
	if (existing)
		ft_update_existing_var(existing, key, value, append);
	else
		ft_lstadd_back(env, key, value);
}

int ft_process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		is_append;

    key = NULL;
    value = NULL;
	if (ft_parse_export_arg(arg, &key, &value, &is_append))
	{
		ft_update_env_var(env, key, value, is_append);
		if (key)
			free(key);
		if (value)
			free(value);
		return (0);
	}
	ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
	if (key)
		free(key);
	if (value)
		free(value);
	return (1);
}
